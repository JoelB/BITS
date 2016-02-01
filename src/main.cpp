#include <QApplication>

#include "bitsmw.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BitsMW *w = new BitsMW;
    w->show();
    return app.exec();
}
