#include "addfilm.h"
#include "ui_addfilm.h"

addFilm::addFilm(QWidget *parent) :
    QWidget(parent),
    d(QSqlDatabase::addDatabase("QPSQL")),
    ui(new Ui::addFilm)
{
    ui->setupUi(this);

    if(!createConnection())
            return;


}

addFilm::~addFilm()
{
    delete ui;
}

bool addFilm::insert(){

    QSqlQuery q;
    QString str;

    str = "INSERT INTO MOVIES (NAME, LENGTH, COUNTRY, DIRECTOR, YEAR, PICTURE) "
          "VALUES(" + f.Name + ", " + f.Length + ", " + f.Country + ", " +
            f.Director + ", " + f.Year + ", " + f.posterBA + ");";

    QString str1;

    //q.prepare("insert into movies(picture) values( :posterBA);");
    q.bindValue(":posterBA", f.posterBA);
    if(!q.exec())
    {
        QMessageBox::warning(0, "Ошибка записи", q.lastError().text());
        return 0;
    }

    qDebug() << "Query success";
    return 1;
}

bool addFilm::createConnection(){

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

void addFilm::on_pbInsert_clicked()
{
    f.Name = "Test Name";
    f.Country = "Test Country";
    f.Length = "120 minutes";
    f.Year = "2020";
    f.Director = "Test Director";

    f.fileName = "D:/qt_projects/db/Blue-Velvet.jpg";
    if(!f.posterPM.load(f.fileName)){
        qDebug() << "Unable to load image";
        return;
    }

    QBuffer buffer(&f.posterBA);
    if(!buffer.open(QIODevice::WriteOnly))
    {
        qDebug() << "Open buffer error";
        return;
    }
    if(!f.posterPM.save(&buffer, "JPG"))
    {
        qDebug() << "Save error";
        return;
    }

    buffer.close();

    insert();
}
