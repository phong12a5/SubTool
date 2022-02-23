#include "processdownload.h"
#include "ui_processdownload.h"

ProcessDownload::ProcessDownload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessDownload)
{
    ui->setupUi(this);
}

ProcessDownload::~ProcessDownload()
{
    delete ui;
}

bool ProcessDownload::setValue(int value)
{
    if(0<= value && value <=100) {
        ui->progressBar->setValue(value);
        return true;
    } else {
        return false;
    }
}

void ProcessDownload::setTitle(QString title)
{
    ui->label->setText(title);
}
