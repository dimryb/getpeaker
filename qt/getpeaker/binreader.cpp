#include "binreader.h"

#include <QFile>
#include <QDebug>
#include <QFileSystemModel>
#include <QtCore/qmath.h>

BinReader::BinReader(QObject *parent) : QObject(parent)
{
    QString path2 = QDir::currentPath() + "/surface.bin";
    QFile file(path2);
    qDebug() << path2;


    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        qDebug() << file.size();
        enum{data_size = 32768};
        static char data[data_size];
        qDebug() << file.read((char*)data, data_size);

        QString string;
        char buffer [50];
        for(int i = 0; i < data_size; i+=2){
            int16_t level = qSqrt(data[i] + data[i+1]);
            //snprintf (buffer, 50, "I%02d M%02d L%04d ", data[i], data[i+1], level);
            snprintf (buffer, 50, "0x%04d, ", level);
            string.append(buffer);
            if ((i+1) % 32 == 31){
                qDebug() << string;
                string.clear();
            }
        }
        qDebug() << string;
        file.close();
    }else{
        qDebug() << "Error open file";
    }
}
