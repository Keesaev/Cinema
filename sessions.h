#ifndef SESSIONS_H
#define SESSIONS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QDate>
#include <QCalendarWidget>
#include <QDateTime>
#include <QSqlRecord>
#include <QVector>
#include <QTableWidgetItem>

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
};

#endif // SESSIONS_H
