#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/icon/myicon/cover.png");
    QSplashScreen splash(pixmap);
    splash.show();

    MainWindow w;
    w.show();

    splash.finish(&w);

    return a.exec();
}
