
extern "C" {
#include "getpeaker.h"
#include "getCPUTime.h"
}

#include <QtDebug>
#include <QTime>
#include <QElapsedTimer>

void _assert(const char *assertion, int line){
    qDebug() << "ASSERT: " << assertion << ", line: " << line;
}
#define ASSERT(cond) ((cond) ? static_cast<void>(0) : _assert(#cond, __LINE__))

static const int16_t testData[] = {
#include "test.data"
};

static uint32_t outIndexRD[2500];

void test_1(){
    enum{
        r_size = 10,
        d_size = 10
    };
    static const int16_t rd[r_size * d_size] = {
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0010, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0010, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0010, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0010, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
    };
    uint16_t numPeak = getPeak4((int16_t*)rd, r_size, 5, outIndexRD);
    qDebug() << "test1 ";
    qDebug() << "number peaks: " << numPeak;
    qDebug() << "";
    Q_ASSERT(numPeak == 2);

}

void test_2(){
    enum{
        r_size = 10,
        d_size = 10
    };
    static const uint16_t rd[r_size * d_size] = {
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0010, 0x00A1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0xfC00, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0001, 0x0002, 0x0005, 0x0002, 0x0001, 0x0002, 0x0001, 0x0005, 0x0002, 0x0001,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0011, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0010, 0x0000
    };

    uint16_t numPeak = getPeak4((int16_t*)rd, r_size, d_size, outIndexRD);
    qDebug() << "test2 ";
    qDebug() << "number peaks: " << numPeak;
    for (int i = 0; i < numPeak; ++i){
        qDebug() << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff);
    }
    qDebug() << "";
    ASSERT(numPeak == 2);
    ASSERT(((outIndexRD[0] >> 16) == 1) && ((outIndexRD[0] & 0xffff) == 1));
    ASSERT(((outIndexRD[1] >> 16) == 8) && ((outIndexRD[1] & 0xffff) == 8));
}

enum{
    r_size = 128,
    d_size = 256
};


double startTime, endTime;

void test_RealData3(){
    startTime = getCPUTime( );
    uint16_t numPeak = getPeak3((int16_t*)testData, r_size, d_size, outIndexRD);
    endTime = getCPUTime( );
    qDebug() << "Time test2: " << endTime-startTime;
    qDebug() << "number peaks: " << numPeak;
    for (int i = 0; i < numPeak; ++i){
        //qDebug() << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff);
    }
}

void test_RealData4(){
    startTime = getCPUTime( );
    uint16_t numPeak = getPeak4((int16_t*)testData, r_size, d_size, outIndexRD);
    endTime = getCPUTime( );
    qDebug() << "Time test1: " << endTime-startTime;
    qDebug() << "number peaks: " << numPeak;
    for (int i = 0; i < numPeak; ++i){
        //qDebug() << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff);
    }
}

void tests (){
    test_1();
    test_2();
    test_RealData3();
    test_RealData4();


}
