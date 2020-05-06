#include "booking.h"
#include "ui_booking.h"

booking::booking(int id_s, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::booking)
{
    id_session = id_s;

    ui->setupUi(this);
    scene = new drawCinema(1, this);

    ui->gridLayout->geometry().setWidth(scene->width);
    ui->gridLayout->geometry().setHeight(scene->height);
    ui->gridLayout->geometry().setCoords(0, 0, 800, 600);
    ui->gridLayout->addWidget(scene);
}

booking::~booking()
{
    delete scene;
    delete ui;
}

void booking::on_pushButton_clicked()
{

}

void booking::on_pushButton_2_clicked()
{
    this->close();
}
