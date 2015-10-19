#include <QtGui/QApplication>
#include "camcapture.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    camCapture w;
    w.show();
    return a.exec();
}
