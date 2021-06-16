#include "mainwindow.h"
#include <QApplication>
#include <QStatusBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    //w.statusBar()->hide(); // for bottom bar
    w.show();

    return a.exec();
}
