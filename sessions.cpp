#include "sessions.h"
#include "ui_sessions.h"

sessions::sessions(int id, QSqlDatabase *d, QWidget *parent) :
    QWidget(parent),
    ds(*d),
    ui(new Ui::sessions)

{
    ui->setupUi(this);

    if(!ds.isOpen())
        if(!createConnection())
            close();

    int privilege = 1;
    qDebug() << "Id " << id;

    if(!getEmployee(id, privilege))
    {
        close();
    }

    if(privilege == 1){
        ui->pbAddSession->setEnabled(0);
        ui->pbAddSession->setVisible(0);

        ui->pbRooms->setEnabled(0);
        ui->pbMovies->setEnabled(0);
        ui->pbAddSession->setEnabled(0);
        ui->pbDeleteSession->setEnabled(0);

        ui->pbRooms->setVisible(0);
        ui->pbMovies->setVisible(0);
        ui->pbAddSession->setVisible(0);
        ui->pbDeleteSession->setVisible(0);
    }
    else{
        m = new movies(&ds);
        r = new rooms(&ds);

        connect(r, SIGNAL(signalDeleted()), this, SLOT(dateSelected()));
        connect(m, SIGNAL(signalDeleted()), this, SLOT(dateSelected()));
    }

    calendar = new QCalendarWidget();

    ui->gridLayout->addWidget(calendar);

    connect(calendar, SIGNAL(selectionChanged()), this, SLOT(dateSelected()));

    initTable();

    calendar->setSelectedDate(QDate::currentDate());
}

// Шапка таблицы и прочие настройки

void sessions::initTable(){
    QStringList headersList;
    headersList << "Сессия" << "Зал" << "Стоимость билета" <<
           "Время" << "Фильм";
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(headersList);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
}

// Запрос

bool sessions::getSessions(QDate date){
    q.clear();
    QString str;
    str = "SELECT SESSIONS.ID_SESSION, "
          "SESSIONS.ID_ROOM, SESSIONS.DATE, "
          "SESSIONS.PRICE, MOVIES.NAME FROM SESSIONS, MOVIES "
          "WHERE MOVIES.ID_MOVIE = SESSIONS.ID_MOVIE AND "
          "EXTRACT(DAY FROM DATE) =" +
            QString::number(date.day()) + " AND EXTRACT(MONTH FROM DATE) =" +
            QString::number(date.month()) + " AND EXTRACT(YEAR FROM DATE) =" +
            QString::number(date.year()) + ";";

    if(!q.exec(str)){
        QMessageBox::warning(0, "Ошибка зароса", q.lastError().text());
        return 0;
    }
    return 1;
}

// Переопределяем таблицу и вектор item'ов, читаем результат запроса

bool sessions::displaySessions(){

    ui->tableWidget->clear();
    initTable();
    for(int i = 0; i < items.size(); i++){
        QVector<QTableWidgetItem*>().swap(items[i]);
        items[i].shrink_to_fit();
    }
    QVector<QVector<QTableWidgetItem*>>().swap(items);
    items.clear();
    items.shrink_to_fit();

    QSqlRecord  rec = q.record();

    int id_session, id_room, price;
    QString movie_name;
    QDateTime dt;

    int i = 0;

    while(q.next())
    {
        items.push_back(QVector<QTableWidgetItem*>());

        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);

        id_session = q.value(rec.indexOf("ID_SESSION")).toInt();
        id_room = q.value(rec.indexOf("ID_ROOM")).toInt();
        price = q.value(rec.indexOf("PRICE")).toInt();
        dt = q.value(rec.indexOf("DATE")).toDateTime();
        movie_name = q.value(rec.indexOf("NAME")).toString();

        for(int j = 0; j < 5; j++){
            items[i].push_back(new QTableWidgetItem());
        }

        items[i][0]->setText(QString::number(id_session));
        items[i][1]->setText(QString::number(id_room));
        items[i][2]->setText(QString::number(price));
        items[i][3]->setText(dt.time().toString());
        items[i][4]->setText(movie_name);


        for(int j = 0; j < 5; j++){
            ui->tableWidget->setItem(i, j, items[i][j]);
        }

        i++;
    }
    return 1;
}

bool sessions::getEmployee(int id, int &priv){
    q.clear();
    q.prepare("SELECT * FROM EMPLOYEE_INFO WHERE ID_EMPLOYEE = :id");
    q.bindValue(":id", id);

    if(!q.exec()){
        QMessageBox::warning(0, "Ошибка зароса", q.lastError().text());
        return 0;
    }

    QSqlRecord rec = q.record();
    QString str;

    while(q.next()){
        priv = q.value(rec.indexOf("privilege")).toInt();
        str = q.value(rec.indexOf("Name")).toString() + " " +
                q.value(rec.indexOf("Second_Name")).toString() + " " +
                q.value(rec.indexOf("Job")).toString();
    }

    ui->leEmployee->setText(str);

    return 1;
}

// Соединение с бд

bool sessions::createConnection(){

    ds.setDatabaseName("cinema");
    ds.setUserName("keesaev");
    ds.setPassword("Admin1");

    if(!ds.open()){
        QMessageBox::warning(0, "Не удалось соединиться с базой данных в sessions", ds.lastError().text());
        return 0;
    }

    qDebug() << "Sessions connection success";
    return 1;
}

                // SLOTS //

void sessions::dateSelected(){

    ui->tableWidget->clear();

    if(!getSessions(calendar->selectedDate()))
    {
        return;
    }
    else
        displaySessions();
}

// ЗАБРОНИРОВАТЬ

void sessions::on_pbBook_clicked()
{
    if(ui->tableWidget->selectedItems().isEmpty())
    {
        qDebug() << "None selected";
        QMessageBox::warning(0, "Выберите сеанс", "Выберите сеанс из списка");
        return;
    }
    else
    {
        qDebug() << ui->tableWidget->selectedItems()[0]->text();
        b = new booking(ui->tableWidget->selectedItems()[0]->text().toInt(), &ds);
        b->show();
    }
}

void sessions::on_pbExit_clicked()
{
    this->close();
}

void sessions::on_pbMovies_clicked()
{
    m->show();
}

void sessions::on_pbRooms_clicked()
{
    r->show();
}

void sessions::on_pbAddSession_clicked()
{
    as = new addSession();

    connect(as, SIGNAL(sessionCreated()), this, SLOT(dateSelected()));
    as->show();
}

void sessions::on_pbDeleteSession_clicked()
{
    if(ui->tableWidget->selectedItems().isEmpty())
    {
        qDebug() << "None selected";
        QMessageBox::warning(0, "Выберите сеанс", "Выберите сеанс из списка");
        return;
    }

    q.clear();
    q.prepare("DELETE FROM SESSIONS WHERE ID_SESSION = :id_session;");

    q.bindValue(":id_session", ui->tableWidget->selectedItems()[0]->text());

    if(!q.exec())
    {
        QMessageBox::warning(0, "Ошибка зароса", q.lastError().text());
        return;
    }

    dateSelected();
}

sessions::~sessions()
{
    for(int i = 0; i < items.size(); i++)
        QVector<QTableWidgetItem*>().swap(items[i]);
    QVector<QVector<QTableWidgetItem*>>().swap(items);
    items.clear();
    delete ui;
    delete m;
    delete b;
    delete r;
    delete as;
}
