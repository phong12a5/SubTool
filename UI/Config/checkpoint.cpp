#include "checkpoint.h"
#include "ui_checkpoint.h"

CheckPoint::CheckPoint(QWidget *parent) :
    SubWindow(parent),
    ui(new Ui::CheckPoint)
{
    ui->setupUi(this);
}

CheckPoint::~CheckPoint()
{
    delete ui;
}

QString CheckPoint::getApiToken()
{
    return ui->lineEdit->text();
}

QString CheckPoint::getApiPhone()
{
    return ui->lineEdit_2->text();
}

QString CheckPoint::getApiEmail()
{
    return ui->lineEdit_3->text();
}
