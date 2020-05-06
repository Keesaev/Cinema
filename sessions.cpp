#include "sessions.h"
#include "ui_sessions.h"

sessions::sessions(QWidget *parent) :
    QWidget(parent),
    d(QSqlDatabase::addDatabase("QPSQL")),
    ui(new Ui::sessions)

{
    ui->setupUi(this);
    calendar = new QCalendarWidget();
    ui->gridLayout->addWidget(calendar);

    if(!createConnection())
        close();

    connect(calendar, SIGNAL(selectionChanged()), this, SLOT(dateSelected()));

    initTable();

    QDate testDate(2020, 5, 21);
    calendar->setSelectedDate(testDate);
}

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

bool sessions::getSessions(QDate date){
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
        qDebug() << "Query error " + q.lastError().text();
        return 0;
    }
    qDebug() << "Successful query";

    return 1;
}

bool sessions::displaySessions(){

    ui->tableWidget->clear();
    initTable();

    for(int i = 0; i < items.size(); i++)
        QVector<QTableWidgetItem*>().swap(items[i]);
    QVector<QVector<QTableWidgetItem*>>().swap(items);
    items.clear();

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

        qDebug() << id_session << " " << id_room << " " <<
                    price << " " << dt << " " << movie_name;

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

bool sessions::createConnection(){

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

                // SLOTS //

void sessions::dateSelected(){
    qDebug() << calendar->selectedDate().day();
    if(!getSessions(calendar->selectedDate()))
    {
        return;
    }
    else
        displaySessions();
}

void sessions::on_pbBook_clicked()
{
    if(ui->tableWidget->selectedItems().isEmpty())
    {
        qDebug() << "None selected";
        QMessageBox::warning(0, "Выберите сеанс", "Выберите сеанс из списка");
    }
    else
    {
        qDebug() << ui->tableWidget->selectedItems()[0]->text();
        booking *b = new booking(ui->tableWidget->selectedItems()[0]->text().toInt());
        b->show();
    }
}

void sessions::on_pbExit_clicked()
{
    this->close();
}

sessions::~sessions()
{
    for(int i = 0; i < items.size(); i++)
        QVector<QTableWidgetItem*>().swap(items[i]);
    QVector<QVector<QTableWidgetItem*>>().swap(items);
    items.clear();
    delete ui;
}
