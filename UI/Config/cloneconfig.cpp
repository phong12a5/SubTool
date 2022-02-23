#include "cloneconfig.h"
#include "ui_cloneconfig.h"
#include <QDebug>


CloneConfig::CloneConfig(QWidget *parent) :
    SubWindow(parent),
    ui(new Ui::CloneConfig)
{
    ui->setupUi(this);
}

CloneConfig::~CloneConfig()
{
    delete ui;
}
