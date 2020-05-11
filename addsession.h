#ifndef ADDSESSION_H
#define ADDSESSION_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

#include <QMessageBox>
#include <QDebug>

#include <QVector>
#include <QTableWidgetItem>

namespace Ui {
class addSession;
}

class addSession : public QWidget
{
    Q_OBJECT

public:
    explicit addSession(QWidget *parent = nullptr);
    ~addSession();

    QVector<QVector<QTableWidgetItem*>> movies;
    QVector<QVector<QTableWidgetItem*>> rooms;

    void initRooms();

    void initMovies();

    bool getMovies();

    bool getRooms();

private slots:
    void on_pbExit_clicked();

    void on_pbInsert_clicked();

private:
    Ui::addSession *ui;
public:
    signals:

    void sessionCreated();
};

#endif // ADDSESSION_H
