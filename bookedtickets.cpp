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
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setAlignment(Qt::AlignCenter);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    view->setMinimumSize(200, 100);
    view->setScene(scene);

    scene->setSceneRect(0, 0, view->width(), view->height());

    ui->gridLayout->addWidget(view);

    bookedIDs = inpBookedIDs;

    getTicketsInfo();
    drawTicket();
}

void bookedTickets::drawTicket(){ // Отрисовываем конкретный билет из вектора tickets
                                        // на графической сцене
    if(tickets.isEmpty()){
        getTicketsInfo();
    }

    QBrush brush;
    QPen pen;

    pen.setColor(QColor(254, 241, 187));
    brush.setColor(QColor(254, 241, 187));
    brush.setStyle(Qt::BrushStyle::SolidPattern);

    scene->addRect(0, 0, scene->width(), scene->height(), pen, brush);
    scene->addItem(tickets[id]);
}

bool bookedTickets::getTicketsInfo(){   // SQL запрос и запись данных в tickets

    if(!tickets.isEmpty()){
        for(auto& o : tickets)
            delete o;
        tickets.clear();
        tickets.shrink_to_fit();
    }

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

        tickets.push_back(new QGraphicsItemGroup());

        while(q.next()){

            QString data;
            QFont f;
            f.setPixelSize(20);
            f.setFamily("Oswald");

            tickets[i]->setY(0);
            tickets[i]->addToGroup(scene->addText("Билет \t\t\t№" + QString::number(bookedIDs[i]), f));

            tickets[i]->setY(20);
            QDateTime dt;
            dt = q.value(rec.indexOf("DATE")).toDateTime();
            tickets[i]->addToGroup(scene->addText(dt.date().toString() + "\t" +
                                                  "\tв " + dt.time().toString() + "\n", f));

            tickets[i]->setY(40);
            data = q.value(rec.indexOf("PRICE")).toString();
            tickets[i]->addToGroup(scene->addText("Стоимость билета\t" + data + " рублей \n", f));

            tickets[i]->setY(60);
            data = q.value(rec.indexOf("LENGTH")).toString();
            tickets[i]->addToGroup(scene->addText("Продолжительность\t" + data + "\n", f));

            tickets[i]->setY(80);
            data = QString::number(q.value(rec.indexOf("COL")).toInt() + 1);
            tickets[i]->addToGroup(scene->addText("Место\t\t\t" + data + "\n", f));

            tickets[i]->setY(100);
            data = QString::number(q.value(rec.indexOf("ROW")).toInt() + 1);
            tickets[i]->addToGroup(scene->addText("Ряд\t\t\t" + data + "\n", f));

            tickets[i]->setY(120);
            data = q.value(rec.indexOf("ID_ROOM")).toString();
            tickets[i]->addToGroup(scene->addText("Зал\t\t\t" + data + "\n", f));

            f.setPixelSize(30);
            tickets[i]->setY(150);

            data = q.value(rec.indexOf("NAME")).toString();
            tickets[i]->addToGroup(scene->addText(data + "\n", f));

            tickets[i]->setY(190);
            f.setPixelSize(40);
            tickets[i]->addToGroup(scene->addText("Кинотеатр Балифаж", f));
        }
    }

    qDebug() << "getTickets success";
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

// Листаем влево и вправо, отрисовываем разные билеты

void bookedTickets::on_pbLeft_clicked()
{
    scene->removeItem(tickets[id]);
    scene->clear();

    id--;

    if(id < 0 || id >= bookedIDs.size())
        id = bookedIDs.size() - 1;
    drawTicket();
}

void bookedTickets::on_pbRight_clicked()
{
    scene->removeItem(tickets[id]);
    scene->clear();

    id++;

    if(id < 0 || id >= bookedIDs.size())
        id = 0;
    drawTicket();
}

void bookedTickets::on_pbExit_clicked()
{
    this->close();
}

void bookedTickets::on_pbSaveAll_clicked()
{

    QString url = QFileDialog::getExistingDirectory(0, "Сохранить",
                                               "/ c:", QFileDialog :: ShowDirsOnly | QFileDialog :: DontResolveSymlinks);

    if(url == ""){
        QMessageBox::warning(this, "Ошибка","Введите путь к файлу");
        return;
    }

    for(int i = 0; i < bookedIDs.size(); i++){

        scene->removeItem(tickets[id]);
        scene->clear();

        id = i;
        drawTicket();

        QImage image(scene->width(), scene->height(), QImage::Format_RGBA8888_Premultiplied);

        QPainter painter(&image);
        scene->render(&painter);
        QImageWriter writer(url + "_" + QString::number(bookedIDs[i]) + ".jpg");

        if(!writer.write(image))
        {
            QString er = writer.errorString();
            QMessageBox::warning(this, "Ошибка", "Не удалось сохранить изображение " + er);
        }
    }
}

bookedTickets::~bookedTickets()
{
    delete ui;
    delete scene;
    delete view;

    for(auto& o : tickets)
        delete o;
}
