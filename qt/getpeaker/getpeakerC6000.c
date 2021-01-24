#include "getpeakerC6000.h"
#include "limits.h"

enum {COEF_10dB = 10}; // = 10^1

/**
  Результаты компилятора для TMS серии C6000 для этой функции:
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*
;*      Loop found in file               : ../getpeaker.c
;*      Loop source line                 : 10
;*      Loop opening brace source line   : 10
;*      Loop closing brace source line   : 13
;*      Known Minimum Trip Count         : 1
;*      Known Max Trip Count Factor      : 1
;*      Loop Carried Dependency Bound(^) : 2
;*      Unpartitioned Resource Bound     : 2
;*      Partitioned Resource Bound(*)    : 2
;*      Resource Partition:
;*                                A-side   B-side
;*      .L units                     1        1
;*      .S units                     0        0
;*      .D units                     1        1
;*      .M units                     0        0
;*      .X cross paths               0        0
;*      .T address paths             0        0
;*      Logical  ops (.LS)           0        0     (.L or .S unit)
;*      Addition ops (.LSD)          2        2     (.L or .S or .D unit)
;*      Bound(.L .S .LS)             1        1
;*      Bound(.L .S .D .LS .LSD)     2*       2*
;*
;*      Searching for software pipeline schedule at ...
;*         ii = 2  Schedule found with 4 iterations in parallel
;*      Done
;*
;*      Loop will be splooped
;*      Collapsed epilog stages       : 0
;*      Collapsed prolog stages       : 0
;*      Minimum required memory pad   : 0 bytes
;*
;*      Minimum safe trip count       : 1
;*----------------------------------------------------------------------------*

    Тут хотел использовать ИНТРИНСИКИ
    Но ничего более оптимального чем делает компилятор не подобрал
*/
uint16_t calcTreshold_C6000(uint16_t * RD, uint32_t size){
    uint64_t sumA = 0;
    uint64_t sumB = 0;
    for(uint32_t i = 0; i < size/2; ++i){
        sumA += RD[i];
        sumB += RD[i + size/2];
    }
    uint16_t average = (sumA+sumB)/size;
    uint32_t treshold = (uint32_t)average * COEF_10dB;
    treshold = (treshold > SHRT_MAX) ? SHRT_MAX : treshold;
    return treshold;
}

/**
  Результаты компилятора для TMS серии C6000 для этой функции:
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*
;*      Loop found in file               : ../getpeaker.c
;*      Loop source line                 : 34
;*      Loop opening brace source line   : 34
;*      Loop closing brace source line   : 40
;*      Known Minimum Trip Count         : 8
;*      Known Maximum Trip Count         : 8
;*      Known Max Trip Count Factor      : 8
;*      Loop Carried Dependency Bound(^) : 3
;*      Unpartitioned Resource Bound     : 4
;*      Partitioned Resource Bound(*)    : 4
;*      Resource Partition:
;*                                A-side   B-side
;*      .L units                     2        3
;*      .S units                     1        1
;*      .D units                     3        1
;*      .M units                     1        0
;*      .X cross paths               0        0
;*      .T address paths             0        0
;*      Logical  ops (.LS)           0        0     (.L or .S unit)
;*      Addition ops (.LSD)          6        6     (.L or .S or .D unit)
;*      Bound(.L .S .LS)             2        2
;*      Bound(.L .S .D .LS .LSD)     4*       4*
;*
;*      Searching for software pipeline schedule at ...
;*         ii = 4  Schedule found with 6 iterations in parallel
;*      Done
;*
;*      Loop will be splooped
;*      Collapsed epilog stages       : 0
;*      Collapsed prolog stages       : 0
;*      Minimum required memory pad   : 0 bytes
;*
;*      Minimum safe trip count       : 1
;*----------------------------------------------------------------------------*

    Тут хотел использовать ИНТРИНСИКИ
    Но ничего более оптимального чем делает компилятор не подобрал.

    Есть интересный интринсик _dmax2 ищет максимум из 4 16 битных элемента
    и параллельно для других четырех.

    Тут для каждого элемента пробегает все смежные элементы, тоесть возможен
    многократный доступ к внешней памяти, чтобы ускорить необходимо включить кэш L2,
    повторный доступ будет осуществлятся из кэша.

    Также возможно использование буферов в статической памяти в котором будут хранится текущие строки
    с которыми осуществляется работа. Доступ к данным из статического буфера самый быстрый.
    Для выделения статической памяти важно знать максимальные размеры массива.

    Возможно использование совершенно иного алгоритма, например попробовать с порогом на скользящих средних.
    Тогда расчет порога и пороговая обработка будут осуществляться в один проход,
    но тогда порог будет плавать и не будет жестко зафиксирован в 10 дБ от среднего уровня.

    Для дальнейшей оптимизации текущего кода нужно глубже погружаться в линейный ассемблер TMC.

    В серии C6000 есть многоядерные процессоры, возможно лупы распаралелить на несколько ядер.

 */
int16_t getPeak_C6000(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD){
    const int16_t offsetIndexes[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    uint16_t* rd = (uint16_t *)RD; // unsighed 16 bit real data !!!
    uint16_t treshold = calcTreshold_C6000(rd, r_size*d_size);
    int16_t numPeaks = 0;
    for(int16_t d = 0; d < d_size; ++d){
        for(int16_t r = 0; r < r_size; ++r){
            uint16_t val = rd[d * r_size + r];
            int16_t topCnt = 0;
            if (val > treshold){
                for(int16_t i = 0; i < 8; ++i){
                    int16_t d_sh = d + offsetIndexes[i][0];
                    int16_t r_sh = r + offsetIndexes[i][1];
                    uint16_t valAdjacent = ((r_sh < 0)||(d_sh < 0)||(r_sh >= r_size)||(d_sh >= d_size))
                            ? 0 : rd[d_sh * r_size + r_sh];
                    topCnt += (val > valAdjacent);
                }
                outIndexRD[numPeaks] = r << 16 | d;
                numPeaks += (topCnt >> 3);
            }
        }
    }

    return numPeaks;
}
