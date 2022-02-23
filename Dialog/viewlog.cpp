#include "viewlog.h"
#include "ui_viewlog.h"

ViewLog::ViewLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewLog)
{
    ui->setupUi(this);
}

ViewLog::~ViewLog()
{
    delete ui;
}
