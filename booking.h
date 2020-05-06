#ifndef BOOKING_H
#define BOOKING_H

#include <QWidget>
#include "drawcinema.h"

namespace Ui {
class booking;
}

class booking : public QWidget
{
    Q_OBJECT

public:
    explicit booking(int id_s, QWidget *parent = nullptr);
    ~booking();
    QGraphicsView *myGraphicView;
    drawCinema    *scene;

    int id_session;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::booking *ui;
};

#endif // BOOKING_H
