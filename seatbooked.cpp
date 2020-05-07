#include "seatbooked.h"

seatBooked::seatBooked(int x, int y, int w, int h, seat *parent) : seat(x, y, w, h, parent)
{
    i = x;
    j = y;
    width = w;
    height = h;

    str = QString::number(i + 1) + ", " + QString::number(j + 1);

    if(!pm.load(":images/images/seat_booked.png"))
        qDebug() << "Unable to load image seat_booked.png";
}

void seatBooked::mousePressEvent(QGraphicsSceneMouseEvent *event){
}
