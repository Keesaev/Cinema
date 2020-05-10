#ifndef SEAT_H
#define SEAT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QVector>

class seat : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit seat(QObject *parent = nullptr);
    seat(int x, int y, int w, int h, QObject *parent = nullptr);
    ~seat(){}
    int width = 50;
    int height = 50;
    int i;
    int j;

    QVector<QPixmap> pmLib;
    int pmLibi = 0;
    QString str;
    QPixmap pm;

    int pmWidth = 50;

public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    signals:
    void mousePressSignal(int i, int j);
public slots:
};

#endif // SEAT_H
