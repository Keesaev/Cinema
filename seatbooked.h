#ifndef SEATBOOKED_H
#define SEATBOOKED_H

#include <QObject>
#include "seat.h"

class seatBooked : public seat
{
    Q_OBJECT
public:
    explicit seatBooked(int x, int y, int w, int h, seat *parent = nullptr);
    ~seatBooked(){}
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SEATBOOKED_H
