#include "addsession.h"
#include "ui_addsession.h"

addSession::addSession(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addSession)
{
    ui->setupUi(this);

    getRooms();

    getMovies();

    QTime t;
    t.setHMS(21, 0, 0);

    ui->dateTimeEdit->setDate(QDate::currentDate());
    ui->dateTimeEdit->setTime(t);
}

bool addSession::getMovies(){

    QSqlQuery q;

    q.prepare("SELECT ID_MOVIE, NAME, LENGTH FROM MOVIES;");
    if(!q.exec()){
        QMessageBox::warning(0, "Ошибка запроса", q.lastError().text());
        return 0;
    }

    ui->tableWidget->clear();
    initMovies();

    for(int i = 0; i < movies.size(); i++)
        QVector<QTableWidgetItem*>().swap(movies[i]);
    QVector<QVector<QTableWidgetItem*>>().swap(movies);
    movies.clear();
    movies.shrink_to_fit();
    int i = 0;

    QSqlRecord rec = q.record();

    while(q.next()){
        movies.push_back(QVector<QTableWidgetItem*>());
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);

        for(int j = 0; j < 3; j++)
            movies[i].push_back(new QTableWidgetItem);

        movies[i][0]->setText(q.value(rec.indexOf("ID_MOVIE")).toString());
        movies[i][1]->setText(q.value(rec.indexOf("NAME")).toString());
        movies[i][2]->setText(q.value(rec.indexOf("LENGTH")).toString());

        for(int j = 0; j < 3; j++)
            ui->tableWidget->setItem(i, j, movies[i][j]);

        i++;
    }

    return 1;
}

bool addSession::getRooms(){
    QSqlQuery q;

    q.prepare("SELECT * FROM ROOMS;");
    if(!q.exec()){
        QMessageBox::warning(0, "Ошибка запроса", q.lastError().text());
        return 0;
    }

    ui->tableWidget_2->clear();
    initRooms();

    for(int i = 0; i < rooms.size(); i++){
        QVector<QTableWidgetItem*>().swap(rooms[i]);
        rooms[i].shrink_to_fit();
    }
    QVector<QVector<QTableWidgetItem*>>().swap(rooms);
    rooms.clear();
    rooms.shrink_to_fit();
    int i = 0;

    // Читаем результат запроса

    QSqlRecord rec = q.record();

    while(q.next()){
        rooms.push_back(QVector<QTableWidgetItem*>());
        ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount() + 1);

        for(int j = 0; j < 2; j++)
            rooms[i].push_back(new QTableWidgetItem);

        rooms[i][0]->setText(q.value(rec.indexOf("ID_ROOM")).toString());
        int rows = (q.value(rec.indexOf("ROWS")).toInt());
        int cols = (q.value(rec.indexOf("COLS")).toInt());
        rooms[i][1]->setText(QString::number(cols * rows));

        for(int j = 0; j < 2; j++)
            ui->tableWidget_2->setItem(i, j, rooms[i][j]);

        i++;
    }
    return 1;
}

void addSession::initMovies(){
    QStringList headersList;
    headersList << "ID" << "Название" << "Продолжительность";
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(headersList);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
}

void addSession::initRooms(){
    QStringList headersList;
    headersList << "ID" << "Количество мест";
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(2);
    ui->tableWidget_2->setHorizontalHeaderLabels(headersList);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void addSession::on_pbExit_clicked()
{
    this->close();
}

void addSession::on_pbInsert_clicked()
{
    if(ui->tableWidget->selectedItems().isEmpty() ||
            ui->tableWidget_2->selectedItems().isEmpty())
    {
        QMessageBox::warning(0, "Заполните поля",
                             "Выберите фильм и зал из списков и нажмите снова");
        return;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO SESSIONS (ID_MOVIE, ID_ROOM, PRICE, DATE) "
              "VALUES (:id_movie, :id_room, :price, :date);");

    int id_movie, id_room, price;
    QDateTime dt;
    dt.setDate(ui->dateTimeEdit->date());
    dt.setTime(ui->dateTimeEdit->time());

    id_movie = (ui->tableWidget->selectedItems()[0]->text()).toInt();
    id_room = (ui->tableWidget_2->selectedItems()[0]->text()).toInt();

    price = ui->lePrice->text().toInt();

    q.bindValue(":id_movie", id_movie);
    q.bindValue(":id_room", id_room);
    q.bindValue(":price", price);
    q.bindValue(":date", dt);

    if(!q.exec()){
        QMessageBox::warning(0, "Ошибка запроса", q.lastError().text());
        return;
    }

    ui->leResult->setText("Сеанс фильма " + QString::number(id_movie)
                          + " в зале " + QString::number(id_room) + " создан");

    emit sessionCreated();
}

addSession::~addSession()
{
    for(int i = 0; i < rooms.size(); i++)
        QVector<QTableWidgetItem*>().swap(rooms[i]);
    QVector<QVector<QTableWidgetItem*>>().swap(rooms);
    rooms.clear();

    for(int i = 0; i < movies.size(); i++)
        QVector<QTableWidgetItem*>().swap(movies[i]);
    QVector<QVector<QTableWidgetItem*>>().swap(movies);
    movies.clear();

    delete ui;
}
