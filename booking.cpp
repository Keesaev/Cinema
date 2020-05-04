#include "booking.h"
#include "ui_booking.h"

booking::booking(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::booking)
{
    ui->setupUi(this);
    scene = new drawCinema(1, nullptr);

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
