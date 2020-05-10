#ifndef MOVIES_H
#define MOVIES_H

#include <QWidget>

#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QMessageBox>
#include <QDebug>

#include <QVector>
#include <QTableWidgetItem>

#include <QLabel>
#include <QLayoutItem>

namespace Ui {
class movies;
}

class movies : public QWidget
{
    Q_OBJECT

public:
    explicit movies(QSqlDatabase *d, QWidget *parent = nullptr);
    ~movies();
    QSqlDatabase *dm;
    QSqlQuery q;

    QVector<QVector<QTableWidgetItem *>> items;

    bool createConnection();
    void initTable();
    bool getMovies();

private slots:
    void on_pbExit_clicked();

    void on_pbDelete_clicked();

    void on_pbInsert_clicked();

private:
    Ui::movies *ui;
};

#endif // MOVIES_H
