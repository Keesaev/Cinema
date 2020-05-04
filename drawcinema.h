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

#include <QSql>
#include <QSqlQuery>
#include <QSqlError>

class drawCinema : public QGraphicsView
{
    Q_OBJECT
public:
    explicit drawCinema(int id_s, QWidget *parent = nullptr);
    virtual ~drawCinema(){
        /*
        for(int i = 0; i < cols; i++)
            for(int j = 0; j < rows; j++)
            {
                delete seats[i][j];
            }
        seats.clear();*/

        for(int i = 0; i < seats.size(); i++)
            QVector<seat*>().swap(seats[i]);
        QVector<QVector<seat*>>().swap(seats);
        delete scene;
    }

    QGraphicsScene      *scene;
    QSqlDatabase        d;

    int id_session;
    int cols = 15;
    int rows = 10;

    const int width = 800;
    const int height = 600;

    int screenSpace = 100;

    int cellWidth = width / cols;
    int cellHeight = (height - screenSpace)/ rows;

    int innerBorderWidth = cellWidth * 0.1;
    int innerBorderHeight = cellHeight * 0.1;

    void drawScene();

    QVector<QVector<seat*>> seats;

signals:

public slots:
};

#endif // DRAWCINEMA_H
