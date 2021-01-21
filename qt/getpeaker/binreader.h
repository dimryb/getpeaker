#ifndef BINREADER_H
#define BINREADER_H

#include <QObject>

class BinReader : public QObject
{
    Q_OBJECT
public:
    explicit BinReader(QObject *parent = nullptr);

signals:

};

#endif // BINREADER_H
