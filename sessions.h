#ifndef SESSIONS_H
#define SESSIONS_H

#include <QWidget>
#include <QCalendarWidget>
#include <QTableWidgetItem>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QMessageBox>
#include <QDebug>

#include <QDate>
#include <QDateTime>

#include <QVector>

#include "booking.h"
#include "movies.h"
#include "rooms.h"
#include "addsession.h"

namespace Ui {
class sessions;
}

class sessions : public QWidget
{
    Q_OBJECT

public:
    explicit sessions(int privilege, QSqlDatabase *d, QWidget *parent = nullptr);
    ~sessions();

    QSqlDatabase ds;
    QSqlQuery q;
    QCalendarWidget *calendar;
    booking *b;
    movies *m;
    rooms *r;
    addSession *as;

    void initTable();
    bool createConnection();
    bool getSessions(QDate date);
    bool displaySessions();
    bool getEmployee(int id, int &priv);
    QVector<QVector<QTableWidgetItem *>> items;

private:
    Ui::sessions *ui;
protected slots:
    void dateSelected();
private slots:
    void on_pbBook_clicked();
    void on_pbExit_clicked();

    void on_pbMovies_clicked();
    void on_pbRooms_clicked();
    void on_pbAddSession_clicked();
    void on_pbDeleteSession_clicked();
};

#endif // SESSIONS_H
