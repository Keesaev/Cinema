#include "autho.h"
#include "booking.h"
#include "sessions.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*
    Widget w;
    w.setWindowTitle("Введите идентификатор и пароль");
    w.show();
    */

    booking *b = new booking(1, nullptr);
    b->show();

    /*
    sessions s;
    s.show();
    */
    return a.exec();
}
