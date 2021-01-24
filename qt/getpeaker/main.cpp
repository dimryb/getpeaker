#include "mainwindow.h"

#include <QApplication>
#include <QtDebug>
#include "binreader.h"

void tests ();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    tests();

    //BinReader binReader;

    return 0;// a.exec();
}
