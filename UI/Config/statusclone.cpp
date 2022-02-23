#include "statusclone.h"
#include "ui_statusclone.h"

StatusClone::StatusClone(int ordinalNumberOfClone) :
    ui(new Ui::StatusClone)
{
    ui->setupUi(this);
    m_ordinalNumberOfClone = ordinalNumberOfClone;
}

StatusClone::~StatusClone()
{
    delete ui;
}

void StatusClone::closeEvent(QCloseEvent *event)
{
    emit closeStatusSubwindow(m_ordinalNumberOfClone);
}

void StatusClone::reloadView(QStringList listLog)
{
    ui->listWidget->clear();
    if(!listLog.isEmpty()){
        foreach(QString str, listLog)
        {
            ui->listWidget->addItem(str);
        }
    }
}
