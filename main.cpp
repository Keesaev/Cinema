#include "autho.h"
#include "booking.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*
    Widget w;
    w.setWindowTitle("Введите идентификатор и пароль");
    w.show();*/

    booking b;
    b.show();
    return a.exec();
}
