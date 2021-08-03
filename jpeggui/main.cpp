#include "jpeggui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    jpeggui w;
    w.show();
    return a.exec();
}
