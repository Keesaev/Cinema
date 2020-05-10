#include "movies.h"
#include "ui_movies.h"

movies::movies(QSqlDatabase *d, QWidget *parent) :
    QWidget(parent),
    dm(d),
    ui(new Ui::movies)
{
    ui->setupUi(this);
    dm = d;

    if(!dm->isOpen())
    {
        if(!createConnection())
            return;
    }

    initTable();

    getMovies();



}

void movies::initTable(){
    QStringList headersList;
    headersList << "ID" << "Название" << "Продолжительность"
                << "Страна" << "Режиссер" << "Год";
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(headersList);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


bool movies::getMovies(){

    ui->tableWidget->setRowCount(0);

    for(int i = 0; i < items.size(); i++)
        QVector<QTableWidgetItem*>().swap(items[i]);
    QVector<QVector<QTableWidgetItem*>>().swap(items);
    items.clear();

    q.prepare("SELECT * FROM MOVIES ORDER BY ID_MOVIE");

    if(!q.exec()){
        QMessageBox::warning(0, "Ошибка запроса ", q.lastError().text());
        return 0;
    }

    QSqlRecord rec = q.record();

    int i = 0;

    while(q.next()){
        items.push_back(QVector<QTableWidgetItem*>());

        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);

        for(int j = 0; j < 6; j++){
            items[i].push_back(new QTableWidgetItem());
        }

        items[i][0]->setText(q.value(rec.indexOf("ID_MOVIE")).toString());
        items[i][1]->setText(q.value(rec.indexOf("NAME")).toString());
        items[i][2]->setText(q.value(rec.indexOf("LENGTH")).toString());
        items[i][3]->setText(q.value(rec.indexOf("COUNTRY")).toString());
        items[i][4]->setText(q.value(rec.indexOf("DIRECTOR")).toString());
        items[i][5]->setText(q.value(rec.indexOf("YEAR")).toString());

        for(int j = 0; j < 6; j++){
            ui->tableWidget->setItem(i, j, items[i][j]);
        }

        i++;
    }
    return 1;
}

bool movies::createConnection(){

    dm->setDatabaseName("cinema");
    dm->setUserName("keesaev");
    dm->setPassword("Admin1");

    if(!dm->open()){
        QMessageBox::warning(0, "Не удалось соединиться с базой данных в movies", dm->lastError().text());
        return 0;
    }

    qDebug() << "movies connection success";
    return 1;
}

void movies::on_pbExit_clicked()
{
    this->close();
}

void movies::on_pbDelete_clicked()
{
    if(ui->tableWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(0, "Выберите фильм", "Выберите фильм для удаления из списка");
        return;
    }

    qDebug() << ui->tableWidget->selectedItems()[0]->text();

    q.clear();
    q.prepare("DELETE FROM MOVIES WHERE ID_MOVIE = :id_movie");
    q.bindValue(":id_movie", ui->tableWidget->selectedItems()[0]->text());

    if(!q.exec())
    {
        QMessageBox::warning(0, "Ошибка удаления", q.lastError().text());
        return;
    }

    getMovies();
}

void movies::on_pbInsert_clicked()
{
    QString name, length, country, director, year;

    name = ui->leName->text();
    length = ui->leLength->text();
    country = ui->leCountry->text();
    director = ui->leDirector->text();
    year = ui->leYear->text();

    if(!(name.length() * length.length() * country.length() *
         director.length() * year.length()))
    {
        QMessageBox::warning(0, "Невозможно добавить фильм", "Заполните все строки");
        return;
    }

    length += " minutes";

    q.clear();
    q.prepare("INSERT INTO MOVIES (NAME, LENGTH, COUNTRY, DIRECTOR, YEAR) "
              "VALUES(:name, :length, :country, :director, :year)");

    q.bindValue(":name", name);
    q.bindValue(":length", length);
    q.bindValue(":country", country);
    q.bindValue(":director", director);
    q.bindValue(":year", year);

    if(!q.exec())
    {
        QMessageBox::warning(0, "Ошибка записи ", q.lastError().text());
        return;
    }

    getMovies();
}

movies::~movies()
{
    for(int i = 0; i < items.size(); i++)
        QVector<QTableWidgetItem*>().swap(items[i]);
    QVector<QVector<QTableWidgetItem*>>().swap(items);
    items.clear();
    delete ui;
}
