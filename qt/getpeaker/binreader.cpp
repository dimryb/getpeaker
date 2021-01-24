#include "binreader.h"

#include <QFile>
#include <QDebug>
#include <QFileSystemModel>
#include <QtCore/qmath.h>

#include <iostream>

using namespace std;

BinReader::BinReader(QObject *parent) : QObject(parent)
{
    QString path2 = QDir::currentPath() + "/surface.bin";
    QFile file(path2);
    qDebug() << path2;


    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        int32_t file_size = file.size();
        qDebug() << "File size: " << file_size;
        enum{data_size = 32768};
        static char data[data_size];
        qDebug() << file.read((char*)data, data_size);

        uint16_t *data16 = (uint16_t*)data;
        int16_t data16_size = file_size/2;

        char buffer [50];
        for(int16_t i = 0; i < data16_size; ++i){
            //snprintf (buffer, 50, "0x%04X, ", (int32_t)data16[i]);
            snprintf (buffer, 50, "%d, ", (int32_t)data16[i]);
            cout << buffer;
            if (i % 512 == 511){
                cout << endl;
            }
        }
        file.close();
    }else{
        qDebug() << "Error open file";
    }
}
