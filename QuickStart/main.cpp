#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

//    a.installNativeEventFilter(&w);

    w.setWindowFlags(w.windowFlags() &~ Qt::WindowMaximizeButtonHint);// 禁止最大化
    w.show();

    return a.exec();
}
