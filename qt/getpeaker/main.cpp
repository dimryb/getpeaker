#include "mainwindow.h"

#include <QApplication>
#include <QtDebug>
#include <QTime>
#include <QElapsedTimer>

extern "C" {
#include "getpeaker.h"
}

void simpleTest(){
    enum{
        r_size = 1000,
        d_size = 1000
    };
    int16_t RD[r_size *d_size] = {
        0x0000, 0x0001, 0x0003, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0009, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0011, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0008, 0x0010, 0x0009, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0009, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    };

    uint32_t outIndexRD[10];

    {
        QElapsedTimer time;
        time.start();
        uint16_t numPeak = getPeak1(RD, r_size, d_size, outIndexRD);

        qDebug() << "number peaks: " << numPeak;
        for (int i = 0; i < numPeak; ++i){
            qDebug() << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff);
        }
        qDebug() << "time: " << time.elapsed();
    }

    {
        QElapsedTimer time;
        time.start();
        uint16_t numPeak = getPeak2(RD, r_size, d_size, outIndexRD);

        qDebug() << "number peaks: " << numPeak;
        for (int i = 0; i < numPeak; ++i){
            qDebug() << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff);
        }
        qDebug() << "time: " << time.elapsed();
    }

    {
        QElapsedTimer time;
        time.start();
        uint16_t numPeak = getPeak3(RD, r_size, d_size, outIndexRD);

        qDebug() << "number peaks: " << numPeak;
        for (int i = 0; i < numPeak; ++i){
            qDebug() << "peak: " << i << " r: " << (outIndexRD[i] >> 16) << " d: " << (outIndexRD[i] & 0xffff);
        }
        qDebug() << "time: " << time.elapsed();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    simpleTest();

    return a.exec();
}
