#include "mainwindow.h"

#include <QApplication>
#include <QtDebug>

void tests ();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    tests();

    return 0;// a.exec();
}
