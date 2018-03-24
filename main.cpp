#include "myview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myView w;
    w.show();

    return a.exec();
}
