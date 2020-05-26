#ifndef BOOKEDTICKETS_H
#define BOOKEDTICKETS_H

#include <QObject>
#include <QWidget>

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>

#include <QVector>

#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QMessageBox>
#include <QDebug>

namespace Ui {
class bookedTickets;
}

class bookedTickets : public QWidget
{
    Q_OBJECT

public:
    explicit bookedTickets(QVector<int> inpBookedIDs, QSqlDatabase *d1 = nullptr, QWidget *parent = nullptr);
    ~bookedTickets();

    QGraphicsScene *scene;
    QGraphicsView *view;
    QVector<int> bookedIDs;
    QVector<QGraphicsItemGroup> tickets;
    QSqlDatabase *dt;

    bool createConnection();
    bool getTicketsInfo();

private:
    Ui::bookedTickets *ui;
};

#endif // BOOKEDTICKETS_H
