#include <QtGui/QApplication>
#include "textfinder.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TextFInder w;
    w.show();

    return a.exec();
}
