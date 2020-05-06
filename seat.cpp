#include "seat.h"

seat::seat(int x, int y, int w, int h, QObject *parent) : QObject(parent), QGraphicsItem()
{
    i = x;
    j = y;
    width = w;
    height = h;

    str = QString::number(i + 1) + ", " + QString::number(j + 1);
}

QRectF seat::boundingRect() const{
    return QRectF(0, 0, width, height);
}

void seat::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget){
    painter->drawRect(boundingRect());
    painter->drawText(boundingRect(), str);
}

void seat::mousePressEvent(QGraphicsSceneMouseEvent *event){

}
