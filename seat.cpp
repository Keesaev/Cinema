#include "seat.h"

seat::seat(int x, int y, int w, int h, QObject *parent) : QObject(parent), QGraphicsItem()
{
    i = x;
    j = y;
    width = w;
    height = h;

    str = QString::number(j + 1) + ", " + QString::number(i + 1);

    if(!pm.load(":images/images/seat_vacant.png"))
        qDebug() << "Unable to load image seat_vacant.png";
    pmLib.push_back(pm);
    if(!pm.load(":images/images/seat_selected.png"))
        qDebug() << "Unable to load image seat_selected.png";
    pmLib.push_back(pm);

    pm = pmLib[0];

    connect(this, SIGNAL(mousePressSignal(int, int)),
            parent, SLOT(itemClicked(int, int)));
}

QRectF seat::boundingRect() const{
    return QRectF(0, 0, width, height);
}

void seat::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget){
    painter->drawPixmap((width-pmWidth)/2, (height-pmWidth)/2, pmWidth, pmWidth, pm);
    QFont f;
    f.setPixelSize(16);
    painter->setFont(f);
    painter->drawText(boundingRect(), Qt::AlignCenter, str);
}

void seat::mousePressEvent(QGraphicsSceneMouseEvent *event){
    emit mousePressSignal(i, j);

    pmLibi ^= 1;
    pm = pmLib[pmLibi];
}
