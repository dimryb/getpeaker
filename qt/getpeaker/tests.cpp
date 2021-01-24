
extern "C" {
#include "getpeaker.h"
#include "getpeakerC6000.h"
}

#include <iostream>

using namespace std;

static bool test_result = true;
static bool final_result = true;
void _assert(const char *func, const char *assertion, int line){
    cout << "UNIT-ASSERT: " << func << ": " << assertion << ", line: " << line << endl;
    test_result = false;
}

void _init(const char *func){
    cout << "UNIT-TEST: " << func << endl;
    test_result = true;
}

void _result(const char *func){
    cout << "Result " << func << ": " << ((test_result) ? "SUCCESS" : "FAILURE") << endl << endl;
    final_result &= test_result;
    test_result = false;
}

void _final_result(){
    cout << endl;
    cout << "FINAL RESULT: " << ((final_result) ? "SUCCESS" : "FAILURE") << endl;
}

#define UNIT_ASSERT(cond) ((cond) ? static_cast<void>(0) : _assert(__FUNCTION__, #cond, __LINE__))
#define UNIT_INIT() _init(__FUNCTION__)
#define UNIT_RESULT() _result(__FUNCTION__)
#define UNIT_FINAL_RESULT() _final_result();

#define FIELD(val, shift, mask) ((val >> shift)& mask)
#define DEBUG_TESTS 0

enum{
    r_size = 32,
    d_size = 521
};

static const uint16_t testData[r_size*d_size] = {
#include "test.data"
};

static uint32_t outIndexRD[1024];
static uint64_t outIndexRDX[1024];

/**
 * @brief test_1
 *
 * Проверка правильно ли строки и столбцы ориентированы(при правильной ориентации 1 пик против 3)
 *
 */
void test_1(){
    UNIT_INIT();
    enum{
        r_size = 10,
        d_size = 10
    };
    static const uint16_t rd[r_size * d_size] = {
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x00AA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

        0x0000, 0x0000, 0x0000, 0x0000, 0x00AA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x00AA, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00AA, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
    };
    // d_size = 5!
    uint16_t numPeak = getPeak((int16_t*)rd, r_size, 5, outIndexRD);
#if DEBUG_TESTS
    cout << "number peaks: " << numPeak << endl;
#endif
    UNIT_ASSERT(numPeak == 1);
    UNIT_ASSERT((FIELD(outIndexRD[0], 16, 0xffff) == 1) && (FIELD(outIndexRD[0], 0, 0xffff) == 1));
    UNIT_RESULT();
}

/**
 * @brief test_2
 *
 * проверка находит ли 2 пика в известных местах
 *
 */
void test_2(){
    UNIT_INIT();
    enum{
        r_size = 10,
        d_size = 10
    };
    static const uint16_t rd[r_size * d_size] = {
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0010, 0x00AA, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0009, 0x00AA, 0x0000,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0008, 0x0010, 0x0000
    };

    uint16_t numPeak = getPeak((int16_t*)rd, r_size, d_size, outIndexRD);
#if DEBUG_TESTS
    cout << "number peaks: " << numPeak << endl;
    for (int i = 0; i < numPeak; ++i){
        cout << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff) << endl;
    }
#endif
    UNIT_ASSERT(numPeak == 2);
    UNIT_ASSERT((FIELD(outIndexRD[0], 16, 0xffff) == 1) && (FIELD(outIndexRD[0], 0, 0xffff) == 1));
    UNIT_ASSERT((FIELD(outIndexRD[1], 16, 0xffff) == 8) && (FIELD(outIndexRD[1], 0, 0xffff) == 8));
    UNIT_RESULT();
}

/**
 * @brief test_3
 *
 * проверка находит ли пик в углу матрицы
 *
 */
void test_3(){
    UNIT_INIT();
    enum{
        r_size = 5,
        d_size = 3
    };
    static const uint16_t rd[r_size * d_size] = {
        0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
        0x0002, 0x0003, 0x0004, 0x0005, 0x0006,
        0x0003, 0x0004, 0x0005, 0x0006, 0x0110,
    };

    uint16_t numPeak = getPeak((int16_t*)rd, r_size, d_size, outIndexRD);
#if DEBUG_TESTS
    cout << "number peaks: " << numPeak << endl;
    for (int i = 0; i < numPeak; ++i){
        cout << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff) << endl;
    }
#endif
    UNIT_ASSERT(numPeak == 1);
    UNIT_ASSERT((FIELD(outIndexRD[0], 16, 0xFFFF) == 4) && (FIELD(outIndexRD[0], 0, 0xffff) == 2));
    UNIT_RESULT();
}

/**
 * @brief test_treshold_1
 *
 * Тест расчета порога
 *
 */
void test_treshold_1(){
    UNIT_INIT();
    enum{
        r_size = 5,
        d_size = 2
    };
    static const uint16_t rd[r_size * d_size] = {
        0x0004, 0x0002, 0x0004, 0x0002, 0x0004,
        0x0002, 0x0004, 0x0002, 0x0004, 0x0002  // treshold == (4+2)*5/10 * 10dB == 30
    };

    uint16_t treshold = calcTreshold((uint16_t*)rd, r_size*d_size);
#if DEBUG_TESTS
    cout << "treshold: " << treshold << endl;
#endif
    UNIT_ASSERT(treshold == 30);
    UNIT_RESULT();
}


/**
 * @brief test_compare_treshold
 *
 * сравнение результатов расчета порога простой реализации и
 *  с адаптацией под ТМС серии C6000
 *
 */
void test_compare_treshold(){
    UNIT_INIT();
    uint16_t treshold_C6000 = calcTreshold_C6000((uint16_t*)testData, r_size*d_size);
    uint16_t treshold = calcTreshold((uint16_t*)testData, r_size*d_size);
#if DEBUG_TESTS
    cout << "treshold_C6000: " << treshold_C6000 << endl;
    cout << "treshold: " << treshold << endl;
#endif
    UNIT_ASSERT(treshold_C6000 == treshold);
    UNIT_RESULT();
}

/**
 * @brief test_RealData_1
 *
 * проверка находит ли пики в массиве данных подобным реальным
 *
 */
void test_RealData_1(){
    UNIT_INIT();
    enum{
        r_size = 512,
        d_size = 32
    };
    uint16_t numPeak = getPeak((int16_t*)testData, r_size, d_size, outIndexRD);
#if DEBUG_TESTS
    cout << "number peaks: " << numPeak << endl;
    for (int i = 0; i < numPeak; ++i){
        cout << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff) << endl;
    }
#endif
    UNIT_ASSERT(numPeak == 3);
    UNIT_ASSERT((FIELD(outIndexRD[0], 16, 0xffff) == 156) && (FIELD(outIndexRD[0], 0, 0xffff) == 5));
    UNIT_ASSERT((FIELD(outIndexRD[1], 16, 0xffff) == 404) && (FIELD(outIndexRD[1], 0, 0xffff) == 15));
    UNIT_ASSERT((FIELD(outIndexRD[2], 16, 0xffff) == 118) && (FIELD(outIndexRD[2], 0, 0xffff) == 27));
    UNIT_RESULT();
}

/**
 * @brief test_RealData_A
 *
 * сравнение результатов функции поиска пиков простой реализации и
 *  с адаптацией под ТМС серии C6000
 *
 */
void test_compare_RealData(){
    UNIT_INIT();
    uint16_t numPeak_C6000 = getPeak_C6000((int16_t*)testData, r_size, d_size, outIndexRD);
    uint16_t numPeak = getPeak((int16_t*)testData, r_size, d_size, outIndexRD);
#if DEBUG_TESTS
    cout << "number peaks: " << numPeak_C6000 << endl;
    for (int i = 0; i < numPeak; ++i){
        cout << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff) << endl;
    }
#endif
    UNIT_ASSERT(numPeak_C6000 == numPeak);

    UNIT_RESULT();
}

/**
 * @brief test3D_1
 *
 * проверка находит ли пик в известном месте в 3D массиве
 *
 */
void test3D_1(){
    UNIT_INIT();

    enum{
        r_size = 5,
        d_size = 3,
        x_size = 4
    };
    static const uint16_t rdx[r_size * d_size * x_size] = {
        0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
        0x0002, 0x0003, 0x0004, 0x0005, 0x0006,
        0x0003, 0x0004, 0x0005, 0x0006, 0x0032,

        0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
        0x0002, 0x0003, 0x0004, 0x0005, 0x0055,
        0x0003, 0x0004, 0x0005, 0x0006, 0x0055,

        0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
        0x0002, 0x0003, 0x0004, 0x0025, 0x0111, // <--- peak 4,1,2
        0x0003, 0x0024, 0x0035, 0x0066, 0x0110,

        0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
        0x0002, 0x0003, 0x0004, 0x0005, 0x0006,
        0x0003, 0x0004, 0x0005, 0x0006, 0x0040,
    };

    uint16_t numPeak = getPeak3D((uint16_t*)rdx, r_size, d_size, x_size, outIndexRDX);
#if DEBUG_TESTS
    cout << "number peaks: " << numPeak << endl;
    for (int i = 0; i < numPeak; ++i){
        cout << "peak: " << i
                 << " r: " << FIELD(outIndexRDX[i], 32, 0xffff)
                 << " d: " << FIELD(outIndexRDX[i], 16, 0xffff)
                 << " x: " << FIELD(outIndexRDX[i], 0, 0xffff)
                 << endl;
    }
#endif
    UNIT_ASSERT(numPeak == 1);
    UNIT_ASSERT((FIELD(outIndexRDX[0], 32, 0xffff) == 4)
            && (FIELD(outIndexRDX[0], 16, 0xffff) == 1)
            && (FIELD(outIndexRDX[0], 0, 0xffff) == 2));
    UNIT_RESULT();
}

/**
 * @brief test3D_RealData_1
 *
 * Проверка работоспособности 3D на реальных данных
 *
 */
void test3D_RealData_1(){
    UNIT_INIT();

    enum{
        r_size = 32,
        d_size = 32,
        x_size = 16
    };

    UNIT_ASSERT(r_size*d_size*x_size <= sizeof(testData));

    uint16_t numPeak = getPeak3D((uint16_t*)testData, r_size, d_size, x_size, outIndexRDX);
#if DEBUG_TESTS
    cout << "number peaks: " << numPeak << endl;
    for (int i = 0; i < numPeak; ++i){
        cout << "peak: " << i
                 << " r: " << FIELD(outIndexRDX[i], 32, 0xffff)
                 << " d: " << FIELD(outIndexRDX[i], 16, 0xffff)
                 << " x: " << FIELD(outIndexRDX[i], 0, 0xffff)
                 << endl;
    }
#endif
    UNIT_ASSERT(numPeak > 0);

    UNIT_RESULT();
}

void tests (){
    test_1();
    test_2();
    test_3();

    test_RealData_1();

    test_treshold_1();

    test3D_1();
    test3D_RealData_1();

    test_compare_treshold();
    test_compare_RealData();

    UNIT_FINAL_RESULT();
}
