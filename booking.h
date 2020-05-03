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
    explicit booking(QWidget *parent = nullptr);
    ~booking();
    QGraphicsView *myGraphicView;
    drawCinema    *scene;
private:
    Ui::booking *ui;
};

#endif // BOOKING_H
