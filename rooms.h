#ifndef ROOMS_H
#define ROOMS_H

#include <QWidget>
#include <QTableWidgetItem>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QMessageBox>
#include <QDebug>

#include <QVector>

namespace Ui {
class rooms;
}

class rooms : public QWidget
{
    Q_OBJECT

public:
    explicit rooms(QSqlDatabase *d, QWidget *parent = nullptr);
    ~rooms();

    QSqlDatabase *dr;

    QVector<QVector<QTableWidgetItem*>> items;

    void initTable();

    bool getRooms();

    bool createConnection();

private slots:

    void on_pbInsert_clicked();

    void on_pbDelete_clicked();

    void on_pbExit_clicked();

public:
    signals:
    void signalDeleted();

private:
    Ui::rooms *ui;
};

#endif // ROOMS_H
