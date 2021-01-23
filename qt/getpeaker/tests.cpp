
extern "C" {
#include "getpeaker.h"
#include "getpeakerC6000.h"
}

#include <QtDebug>

static bool test_result = true;
static bool final_result = true;
void _assert(const char *func, const char *assertion, int line){
    qDebug() << "UNIT-ASSERT: " << func << ": " << assertion << ", line: " << line;
    test_result = false;
}

void _init(const char *func){
    qDebug() << "UNIT-TEST: " << func;
    test_result = true;
}

void _result(const char *func){
    qDebug() << "Result " << func << ": " << ((test_result) ? "SUCCESS" : "FAILURE");
    qDebug() << "";
    final_result &= test_result;
    test_result = false;
}

void _final_result(){
    qDebug() << "";
    qDebug() << "FINAL RESULT: " << ((final_result) ? "SUCCESS" : "FAILURE");
}

#define UNIT_ASSERT(cond) ((cond) ? static_cast<void>(0) : _assert(__FUNCTION__, #cond, __LINE__))
#define UNIT_INIT() _init(__FUNCTION__)
#define UNIT_RESULT() _result(__FUNCTION__)
#define UNIT_FINAL_RESULT() _final_result();

static const int16_t testData[] = {
#include "test.data"
};

enum{
    r_size = 128,
    d_size = 256
};

static uint32_t outIndexRD[2500];

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
    uint16_t numPeak = getPeak_C6000((int16_t*)rd, r_size, 5, outIndexRD);
    qDebug() << "number peaks: " << numPeak;
    UNIT_ASSERT(numPeak == 1);
    UNIT_RESULT();
}

void test_2(){
    UNIT_INIT();
    enum{
        r_size = 10,
        d_size = 10
    };
    static const uint16_t rd[r_size * d_size] = {
        0xFE00, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0010, 0x00A1, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0009, 0x0011, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0008, 0x0010, 0x0000
    };

    uint16_t numPeak = getPeak_C6000((int16_t*)rd, r_size, d_size, outIndexRD);
    qDebug() << "number peaks: " << numPeak;
    for (int i = 0; i < numPeak; ++i){
        qDebug() << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff);
    }
    UNIT_ASSERT(numPeak == 2);
    UNIT_ASSERT(((outIndexRD[0] >> 16) == 1) && ((outIndexRD[0] & 0xffff) == 1));
    UNIT_ASSERT(((outIndexRD[1] >> 16) == 8) && ((outIndexRD[1] & 0xffff) == 8));
    UNIT_RESULT();
}

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

    uint16_t numPeak = getPeak_C6000((int16_t*)rd, r_size, d_size, outIndexRD);
    qDebug() << "number peaks: " << numPeak;
    for (int i = 0; i < numPeak; ++i){
        //qDebug() << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff);
    }
    UNIT_ASSERT(numPeak == 1);
    UNIT_ASSERT(((outIndexRD[0] >> 16) == 4) && ((outIndexRD[0] & 0xffff) == 2));
    UNIT_RESULT();
}

void test_treshold(){
    UNIT_INIT();
    uint16_t treshold_C6000 = calcTreshold_C6000((int16_t*)testData, r_size, d_size);
    uint16_t treshold = calcTreshold((int16_t*)testData, r_size, d_size);
    qDebug() << "treshold_C6000: " << treshold_C6000;
    qDebug() << "treshold: " << treshold;
    UNIT_ASSERT(treshold_C6000 == treshold);
    UNIT_RESULT();
}


void test_RealData_1(){
    UNIT_INIT();
    uint16_t numPeak = getPeak((int16_t*)testData, r_size, d_size, outIndexRD);
    qDebug() << "number peaks: " << numPeak;
    for (int i = 0; i < numPeak; ++i){
        //qDebug() << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff);
    }
    UNIT_RESULT();
}

void test_RealData_A(){
    UNIT_INIT();
    uint16_t numPeak_C6000 = getPeak_C6000((int16_t*)testData, r_size, d_size, outIndexRD);
    uint16_t numPeak = getPeak((int16_t*)testData, r_size, d_size, outIndexRD);
    qDebug() << "number peaks: " << numPeak_C6000;
    for (int i = 0; i < numPeak; ++i){
        //qDebug() << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff);
    }
    UNIT_ASSERT(numPeak_C6000 == numPeak);
    UNIT_RESULT();
}

void tests (){
    test_1();
    test_2();
    test_3();

    test_treshold();

    test_RealData_1();
    test_RealData_A();

    UNIT_FINAL_RESULT();
}
