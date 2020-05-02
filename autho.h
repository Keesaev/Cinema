#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    bool createConnection();
    int ID;
    QString password;
    QSqlDatabase db;
    QSqlQuery query;
private slots:
    void on_pbExit_clicked();

    void on_pbEnter_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
