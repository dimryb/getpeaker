
extern "C" {
#include "getpeaker.h"
#include "getCPUTime.h"
}

#include <QtDebug>
#include <QTime>
#include <QElapsedTimer>

static const int16_t testData[] = {
#include "test.data"
};

enum{
    r_size = 128,
    d_size = 256
};

static uint32_t outIndexRD[2500];
double startTime, endTime;

void test_RealData1(){
    startTime = getCPUTime( );
    uint16_t numPeak = getPeak1((int16_t*)testData, r_size, d_size, outIndexRD);
    endTime = getCPUTime( );
    qDebug() << "Time test1: " << endTime-startTime;
    qDebug() << "number peaks: " << numPeak;
    for (int i = 0; i < numPeak; ++i){
        //qDebug() << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff);
    }
}

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
    test_RealData1();
    test_RealData3();
    test_RealData4();
}
