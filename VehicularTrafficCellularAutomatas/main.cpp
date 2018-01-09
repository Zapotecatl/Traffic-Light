//#include <QtGui/QApplication>

#include <QApplication>
#include "mainwindow.h"

#include <time.h>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

#include <QtGui>

#if QT_VERSION >= 0x050000
    #include <QtWidgets>
#endif

int main(int argc, char *argv[])
{
    //srand(time(NULL));
    //std::srand(unsigned (std::time(0)));

    std::srand(1000);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

