#include "autho.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("Введите идентификатор и пароль");
    w.show();
    return a.exec();
}
