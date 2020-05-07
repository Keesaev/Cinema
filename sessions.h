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

namespace Ui {
class sessions;
}

class sessions : public QWidget
{
    Q_OBJECT

public:
    explicit sessions(QWidget *parent = nullptr);
    ~sessions();

    QSqlDatabase d;
    QSqlQuery q;
    QCalendarWidget *calendar;

    void initTable();
    bool createConnection();
    bool getSessions(QDate date);
    bool displaySessions();
    QVector<QVector<QTableWidgetItem *>> items;

private:
    Ui::sessions *ui;
protected slots:
    void dateSelected();
private slots:
    void on_pbBook_clicked();
    void on_pbExit_clicked();
    void on_pushButton_clicked();
};

#endif // SESSIONS_H
