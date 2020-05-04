#include "drawcinema.h"

drawCinema::drawCinema(int id_s, QWidget *parent)
    : QGraphicsView(parent),
      d(QSqlDatabase::addDatabase("QPSQL"))
{
    id_session = id_s;

    scene = new QGraphicsScene();

    this->setScene(scene);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setAlignment(Qt::AlignCenter);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->setMinimumSize(width, height);
    scene->setSceneRect(0, 0, width, height);
    qDebug() << id_session;
    drawScene();
}

void drawCinema::drawScene(){
    for(int i = 0; i < cols; i++){
        seats.push_back(QVector<seat*>());
        for(int j = 0; j < rows; j++){
            seat *buffSeat = new seat(i,
                                      j,
                                      cellWidth - innerBorderWidth * 4,
                                      cellHeight - innerBorderHeight * 4);
            seats[i].push_back(buffSeat);
            seats[i][j]->setX(i * cellWidth + innerBorderWidth * 2);
            seats[i][j]->setY(j * cellHeight + innerBorderHeight * 2 +
                              screenSpace);
            scene->addItem(seats[i][j]);
        }
    }
}
