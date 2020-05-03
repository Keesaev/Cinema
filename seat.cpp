#include "seat.h"

seat::seat(QObject *parent) : QObject(parent), QGraphicsItem()
{

}

QRectF seat::boundingRect() const{
    return QRectF(0, 0, width, height);
}

void seat::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget){
    painter->drawRect(boundingRect());
    painter->drawText(boundingRect(), QString::number(i) + ", " +
                      QString::number(j));
}

void seat::mousePressEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "Clicked on: "<< i << " " << j;
}
