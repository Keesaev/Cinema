#ifndef SEAT_H
#define SEAT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

class seat : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit seat(QObject *parent = nullptr);
    seat(int x, int y, int w, int h, QObject *parent = nullptr);

    int width = 50;
    int height = 50;
    int i;
    int j;

    QString str;

public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:

public slots:
};

#endif // SEAT_H
