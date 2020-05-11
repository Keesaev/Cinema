#include "autho.h"
#include "ui_autho.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget),
      d(QSqlDatabase::addDatabase("QPSQL"))
{
    ui->setupUi(this);

    if(!createConnection())
    {
        ui->pbExit->setEnabled(0);
        ui->pbEnter->setEnabled(0);
    }
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::createConnection(){

    d.setDatabaseName("cinema");
    d.setUserName("keesaev");
    d.setPassword("Admin1");

    if(!d.open()){
        QMessageBox::warning(0, "Не удалось соединиться с базой данных", d.lastError().text());
        return 0;
    }

    qDebug() << "Connection success";
    return 1;
}

void Widget::on_pbExit_clicked()
{
    this->close();
}

void Widget::on_pbEnter_clicked()
{
    ID = ui->leID->text().toInt();
    password = ui->lePass->text();

    if(!query.exec("SELECT * FROM users;")){
        qDebug() << "Unable to execute query" << query.lastError();
        return;
    }

    QSqlRecord rec = query.record();

    QString qPass;

    while(query.next()){
        if(query.value(rec.indexOf("id")).toInt() == ID ||
                query.value(rec.indexOf("password")).toString() == password){
            qDebug() << "Success";
            sessions *s = new sessions(&d);
            s->show();

            this->close();

            return;
        }
    }

    QMessageBox::warning(0, "Неверные данные",
                         "Введенный идентификатор или пароль неверен");
}
