#include "interactconfig.h"
#include "ui_interactconfig.h"
#include <QDebug>
#include <QDesktopServices>

InteractConfig::InteractConfig(QWidget *parent) :
    SubWindow(parent),
    ui(new Ui::InteractConfig)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    if(SQLiteWorker::getInstance()->getNumberThread() != NULL)
    {
        ui->numberThread->setValue(SQLiteWorker::getInstance()->getNumberThread());
    }
    else
        ui->numberThread->setValue(1);

    if(SQLiteWorker::getInstance()->getProfilePath() != NULL)
        ui->profileLink->setText(SQLiteWorker::getInstance()->getProfilePath());

}

InteractConfig::~InteractConfig()
{
    delete ui;
}

void InteractConfig::updateBaseCofig()
{
    baseConfig.profileLink = ui->profileLink->text();
}

void InteractConfig::updateThread()
{
    thread.numberThread = std::stoi(ui->numberThread->text().toStdString());
}

BaseConfig *InteractConfig::getBaseConfig()
{
    updateBaseCofig();
    return &baseConfig;
}

Thread *InteractConfig::getThread()
{
    updateThread();
    return &thread;
}


void InteractConfig::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    qDebug() << "folder name " << dir;
    ui->profileLink->setText(dir);
}

