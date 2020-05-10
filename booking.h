#ifndef BOOKING_H
#define BOOKING_H

#include <QWidget>
#include "drawcinema.h"

#include <QVector>
#include <QPair>

#include <QSqlDatabase>
#include <QSql>
#include <QSqlError>
#include <QSqlRecord>

namespace Ui {
class booking;
}

class booking : public QWidget
{
    Q_OBJECT

public:
    explicit booking(int id_s, QSqlDatabase *d, QWidget *parent = nullptr);
    ~booking();
    QGraphicsView *myGraphicView;
    drawCinema    *scene;
    QSqlDatabase  *db;


    int id_session;
    QVector<QPair<int, int>> bookedSeats;

    bool createConnection();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void itemClicked(int i, int j);

private:
    Ui::booking *ui;
    signals:
    void closeSignal();
};

#endif // BOOKING_H
