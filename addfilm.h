#ifndef ADDFILM_H
#define ADDFILM_H

#include <QWidget>
#include <QLabel>

#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QMessageBox>
#include <QDebug>

#include <QDateTime>

#include <QPixmap>
#include <QByteArray>
#include <QBuffer>

namespace Ui {
class addFilm;
}

class addFilm : public QWidget
{
    Q_OBJECT

public:
    explicit addFilm(QWidget *parent = nullptr);
    ~addFilm();

    QSqlDatabase d;


    struct film{
        QString Name, Length, Country, Director, fileName;
        QString Year;
        QByteArray posterBA;
        QPixmap posterPM;
    };

    film f;

    bool insert();
    bool createConnection();
private slots:
    void on_pbInsert_clicked();

private:
    Ui::addFilm *ui;
};

#endif // ADDFILM_H
