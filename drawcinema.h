#ifndef DRAWCINEMA_H
#define DRAWCINEMA_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include "seat.h"
#include <QVector>
#include <QDebug>

class drawCinema : public QGraphicsView
{
    Q_OBJECT
public:
    explicit drawCinema(QWidget *parent = nullptr);
    drawCinema(int c = 10, int r = 15, QWidget *parent = nullptr);
    virtual ~drawCinema(){
        /*
        for(int i = 0; i < cols; i++)
            for(int j = 0; j < rows; j++)
            {
                delete seats[i][j];
            }
        seats.clear();*/
        for(int i = 0; i < cols; i++)
            QVector<seat*>().swap(seats[i]);
        QVector<QVector<seat*>>().swap(seats);
        qDebug() << "size" << seats.size()
                 << "capacity" << seats.capacity();
        delete scene;
    }

    QGraphicsScene      *scene;

    int cols = 5;
    int rows = 5;

    const int width = 800;
    const int height = 600;

    int screenSpace = 100;

    int cellWidth = width / cols;
    int cellHeight = (height - screenSpace)/ rows;

    int innerBorderWidth = cellWidth * 0.1;
    int innerBorderHeight = cellHeight * 0.1;

    QVector<QVector<seat*>> seats;

signals:

public slots:
};

#endif // DRAWCINEMA_H
