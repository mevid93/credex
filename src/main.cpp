#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    int exitCode = a.exec();

    qDebug() << "Application exited with code:" << exitCode;

    return exitCode;
}
