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

    //

    // Соединение с БД

    if(!createConnection())
        return;

    // Получаем метрики зала

    if(!getRoomInfo()){
        return;
    }

    // Получаем коорд-ы

    if(!getBookedInfo())
    {
        return;
    }
    else
        drawScene();
}

void drawCinema::drawScene(){

    // Отрисовываем зал соответственно заранее заполненной матрице

    scene->clear();

    QGraphicsItemGroup *bg = new QGraphicsItemGroup();
    QPixmap bgImg;

    if(!bgImg.load(":images/images/backGround.jpg"))
        qDebug() << "Unable to load bg image";
    else
        qDebug() << "Bg image loaded";

    bg->addToGroup(scene->addPixmap(bgImg));
    bg->setX(0);
    bg->setY(0);

    scene->addItem(bg);

    for(int i = 0; i < cols; i++){
        for(int j = 0; j < rows; j++)
        {
            seats[i][j]->setX(i * cellWidth);
            seats[i][j]->setY((rows - 1 - j) * cellHeight);
            scene->addItem(seats[i][j]);
        }
    }
}

bool drawCinema::getBookedInfo(){

    QSqlQuery q;
    QString str;
    str = "SELECT ROW, COL FROM BOOKED WHERE ID_SESSION = " +
            QString::number(id_session) + ";";

    // Запрашиваем список занятых мест по соответствии с сеансом

    if(!q.exec(str)){
        QMessageBox::warning(0, "Ошибка запроса ", q.lastError().text());
        return 0;
    }

    QSqlRecord rec;
    rec = q.record();

    // В соответствии с рез-тами запроса переопределяем места как занятые

    while(q.next()){
        int i = q.value(rec.indexOf("ROW")).toInt();
        int j = q.value(rec.indexOf("COL")).toInt();

        seats[i][j] = new seatBooked(i,
                                     j,
                                     cellWidth,
                                     cellHeight);
    }

    return 1;
}

bool drawCinema::getRoomInfo(){
    QSqlQuery q;

    QString str;
    str = "SELECT ROWS, COLS FROM ROOMS WHERE ID_ROOM IN "
          " (SELECT ID_ROOM FROM SESSIONS WHERE ID_SESSION = " +
            QString::number(id_session) + ");";

    // Запрашиваем метрики зала

    if(!q.exec(str))
    {
        qDebug() << "Query error " << q.lastError().text();
        return 0;
    }

    // Читаем

    QSqlRecord rec;
    rec = q.record();
    while(q.next()){
        rows = q.value(rec.indexOf("ROWS")).toInt();
        cols = q.value(rec.indexOf("COLS")).toInt();
    }

    qDebug() << "ROWS: " << rows << " COLS: " << cols;

    // Определяем графические метрики

    cellWidth = width / cols;
    cellHeight = (height - 100) / rows;

    // Заполняем места как свободные

    for(int i = 0; i < cols; i++){
        seats.push_back(QVector<seat*>());
        for(int j = 0; j < rows; j++){
            seat *buffSeat = new seat(i,
                                      j,
                                      cellWidth,
                                      cellHeight, this);
            seats[i].push_back(buffSeat);
        }
    }

    return 1;
}

bool drawCinema::createConnection(){
    d.setDatabaseName("cinema");
    d.setUserName("keesaev");
    d.setPassword("Admin1");

    if(!d.open()){
        QMessageBox::warning(0, "Не удалось соединиться с базой данных", d.lastError().text());
        return 0;
    }

    qDebug() << "Connection success";
    return 1;
}

void drawCinema::itemClicked(int i, int j){
    qDebug() << "Click";
    QPainter p;
    scene->update();
    emit itemClickedSignal(i, j);
}
