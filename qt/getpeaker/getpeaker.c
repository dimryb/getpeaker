
#include "getpeaker.h"
#include "limits.h"

enum {COEF_10dB = 10}; // = 10^1

static int16_t calcTreshold(int16_t * RD, int16_t r_size, int16_t d_size){
    int64_t sum = 0;
    for(int16_t r = 0; r < r_size; ++r){
        for(int16_t d = 0; d < d_size; ++d){
            sum += RD[r*d_size + d];
        }
    }
    int16_t average = sum/r_size/d_size;
    int16_t treshold = average * COEF_10dB;
    return treshold;
}

static int16_t calcTreshold2(int16_t * RD, int16_t r_size, int16_t d_size){
    int64_t sum = 0;
    for(int16_t r = 0; r < r_size/2; ++r){
        for(int16_t d = 0; d < d_size; ++d){
            sum += RD[r*d_size + d] + RD[r*d_size + d + r_size/2];
        }
    }
    int16_t average = sum/r_size/d_size;
    int16_t treshold = average * COEF_10dB;
    return treshold;
}

#if 0
int16_t getPeak(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD){
    int16_t numPeaks = 0;
    // Включить кэш

    // Перенести в статический буфер - быстрее доступ чем с кэша

    // Перенести векторные операции
        // локальные максимумы если уровень > 10 Дб

    // Использование интринисков

    // + распоралелить по ядрам
    return numPeaks;
}
#endif

static const int16_t offsetIndexes[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1},           {0, 1},
    {1, -1}, {1, 0}, {1, 1}
};

int16_t getPeak3(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD){
    int16_t numPeaks = 0;
    for(int16_t d = 0; d < d_size; ++d){
        for(int16_t r = 0; r < r_size; ++r){
            int16_t val22 = RD[d * r_size + r];
            int16_t topCnt = 0;
            for(int16_t i = 0; i < 8; ++i){
               int16_t d_sh = d + offsetIndexes[i][0];
               int16_t r_sh = r + offsetIndexes[i][1];
               int16_t val = 0;
               if ((r_sh < 0)||(d_sh < 0)||(r_sh >= r_size)||(d_sh >= d_size)){
                   val = SHRT_MIN;
               }else{
                   val = RD[d_sh * r_size + r_sh];
               }
               if (val22 > val){
                   ++topCnt;
               }else{
                   break;
               }
            }
            if (topCnt == 8){
                outIndexRD[numPeaks] = r << 16 | d;
                ++numPeaks;
            }
        }
    }

    return numPeaks;
}

int16_t getPeak4(int16_t * RD, int16_t r_size, int16_t d_size, uint32_t * outIndexRD){
    int16_t treshold = calcTreshold(RD, r_size, d_size);
    int16_t treshold2 = calcTreshold2(RD, r_size, d_size);
    treshold+=treshold2;

    int16_t numPeaks = 0;
    for(int16_t d = 1; d < d_size-1; ++d){
        for(int16_t r = 1; r < r_size-1; ++r){
            int16_t val = RD[d * r_size + r];
            int16_t topCnt = 0;
            if (val > treshold){
                for(int16_t i = 0; i < 8; ++i){
                    int16_t d_sh = d + offsetIndexes[i][0];
                    int16_t r_sh = r + offsetIndexes[i][1];
                    int16_t valAdjacent = RD[d_sh * r_size + r_sh];
                   if (val > valAdjacent){
                       ++topCnt;
                   }else{
                       break;
                   }
                }
                if (topCnt == 8){
                    outIndexRD[numPeaks] = r << 16 | d;
                    ++numPeaks;
                }
            }
        }
    }

    return numPeaks;
}
