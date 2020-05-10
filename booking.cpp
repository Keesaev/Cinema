#include "booking.h"
#include "ui_booking.h"

booking::booking(int id_s, QSqlDatabase *d, QWidget *parent) :
    QWidget(parent),
    db(d),
    ui(new Ui::booking)
{
    id_session = id_s;

    ui->setupUi(this);

    if(!db->isOpen()){
        if(!createConnection())
            return;
    }

    scene = new drawCinema(id_session, db, this);
    connect(scene, SIGNAL(itemClickedSignal(int, int)), this, SLOT(itemClicked(int, int)));
    ui->gridLayout->addWidget(scene);
    ui->gridLayout->geometry().setWidth(scene->width);
    ui->gridLayout->geometry().setHeight(scene->height);
    ui->gridLayout->geometry().setCoords(0, 0, 800, 600);
}

bool booking::createConnection(){

    db->setDatabaseName("cinema");
    db->setUserName("keesaev");
    db->setPassword("Admin1");

    if(!db->open()){
        QMessageBox::warning(0, "Не удалось соединиться с базой данных в booking", db->lastError().text());
        return 0;
    }

    qDebug() << "Booking Connection success";
    return 1;
}

void booking::on_pushButton_clicked()
{
    if(!bookedSeats.size())
    {
        QMessageBox::warning(0, "Выберите места", "Выберите места для брони кликнув по ним на графике");
        return;
    }

    QSqlQuery q;

    for(int i = 0; i < bookedSeats.size(); i++){
        q.prepare("INSERT INTO BOOKED (ID_SESSION, ROW, COL) "
                  "VALUES (:id_session, :row, :col);");
        q.bindValue(":id_session", id_session);
        q.bindValue(":col", bookedSeats[i].first);
        q.bindValue(":row", bookedSeats[i].second);

        scene->seats[bookedSeats[i].first][bookedSeats[i].second] = new
                seatBooked(bookedSeats[i].first,
                           bookedSeats[i].second,
                           scene->cellWidth,
                           scene->cellHeight);
        scene->seats[bookedSeats[i].first][bookedSeats[i].second]->
                setX(bookedSeats[i].first * scene->cellWidth);
        scene->seats[bookedSeats[i].first][bookedSeats[i].second]->
                setY((scene->rows - 1 - bookedSeats[i].second) * scene->cellHeight);
        scene->scene->addItem(scene->seats[bookedSeats[i].first][bookedSeats[i].second]);

        if(!q.exec())
        {
            qDebug() << "Query error on INSERT" << q.lastError().text();
            return;
        }
    }
    bookedSeats.clear();
    bookedSeats.shrink_to_fit();

    scene->scene->update();

    ui->teBooked->setText("Места забронированы");
}

void booking::on_pushButton_2_clicked()
{
    this->close();
    emit closeSignal();
}

void booking::itemClicked(int i, int j){

    bool idkwtd = 1;

    for(int a = 0; a < bookedSeats.size(); a++){
        if(bookedSeats[a].first == i && bookedSeats[a].second == j)
        {
            qDebug() << "Already present " << i << " " << j;
            bookedSeats.erase(bookedSeats.begin() + a);
            bookedSeats.shrink_to_fit();
            idkwtd = 0;
            break;
        }
    }

    if(idkwtd){
        QPair<int, int> buff(i, j);
        bookedSeats.push_back(buff);
    }

    ui->teBooked->clear();
    ui->teBooked->append("Выбранные места: ");
    for(int a = 0; a < bookedSeats.size(); a++){
        ui->teBooked->append("Ряд " +
                             QString::number(bookedSeats[a].second + 1) +
                             " Место " +
                             QString::number(bookedSeats[a].first + 1));
    }

}

booking::~booking()
{
    scene->~drawCinema();
    delete scene;
    delete ui;
    qDebug() << "destructor booking call";
}
