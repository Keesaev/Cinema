#include "bookedtickets.h"
#include "ui_bookedtickets.h"

bookedTickets::bookedTickets(QVector<int> inpBookedIDs, QSqlDatabase *d1, QWidget *parent) :
    QWidget(parent),
    dt(d1),
    ui(new Ui::bookedTickets)
{
    ui->setupUi(this);

    if(!dt->isOpen()){
        if(!createConnection())
        {
            this->close();
        }
    }

    scene = new QGraphicsScene();
    view = new QGraphicsView();

    view->setAlignment(Qt::AlignCenter);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    bookedIDs = inpBookedIDs;
}

// TODO:
// Интерфейс просмотра билетов - можно скролиить билеты и смотреть какой следующий, какой предыдущий
// Для начала вывод функцию, выводящую один из билетов на scene - drawTicket(int id) по id обращаемся
// к элементам tickets, там уже тупо отображаем на сцену. По нажатию кнопки (вправо-влево) будет вызываться
// та же функция, но с другим айди


bool bookedTickets::getTicketsInfo(){   // SQL запрос и запись данных в QVector<QGraphicsItemGroup>

    tickets.clear();
    tickets.shrink_to_fit();

    for(int i = 0; i < bookedIDs.size(); i++){
        QSqlQuery q;
        q.prepare("SELECT NAME, LENGTH, DATE, PRICE, ID_ROOM, "
                  "ROW, COL FROM TICKET, BOOKED "
                  "WHERE TICKET.ID_SESSION = BOOKED.ID_SESSION "
                  "AND ID_BOOKED = :id_booked");
        q.bindValue(":id_booked", bookedIDs[i]);

        if(!q.exec()){
            QMessageBox::warning(0, "Ошибка запроса ", q.lastError().text());
            return 0;
        }

        QSqlRecord rec = q.record();

        tickets.push_back(QGraphicsItemGroup());

        while(q.next()){
            QString data = q.value(rec.indexOf("NAME")).toString();
            tickets[i].addToGroup(scene->addText(data));

            data = q.value(rec.indexOf("ID_ROOM")).toString();
            tickets[i].addToGroup(scene->addText("Зал " + data));

            data = q.value(rec.indexOf("LENGTH")).toString();
            tickets[i].addToGroup(scene->addText("Продолжительность " + data));

            data = q.value(rec.indexOf("ROW")).toString();
            tickets[i].addToGroup(scene->addText("Ряд " + data));

            data = q.value(rec.indexOf("COL")).toString();
            tickets[i].addToGroup(scene->addText("Место " + data));

            data = q.value(rec.indexOf("PRICE")).toString();
            tickets[i].addToGroup(scene->addText("Стоимость билета" + data));

            data = q.value(rec.indexOf("DATE")).toString();
            tickets[i].addToGroup(scene->addText("Дата и время" + data));

            tickets[i].addToGroup(scene->addText("Билет №" + QString::number(bookedIDs[i])));
        }
    }

    return 1;
}

bool bookedTickets::createConnection(){
    dt->addDatabase("QPSQL");
    dt->setDatabaseName("cinema");
    dt->setUserName("keesaev");
    dt->setPassword("Admin1");

    if(!dt->open()){
        QMessageBox::warning(0, "Не удалось соединиться с базой данных", dt->lastError().text());
        return 0;
    }

    qDebug() << "Connection success";
    return 1;
}

bookedTickets::~bookedTickets()
{
    delete ui;
    delete scene;
    delete view;
}
