#include "viewclone.h"
#include "ui_viewclone.h"
#include "log.h"
#include <QColor>

ViewClone::ViewClone(QWidget *parent) :
    SubWindow(parent),
    ui(new Ui::ViewClone)
{
    ui->setupUi(this);
    initTable();
    reloadTable();
    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this,
                 SLOT(slotSelectionChange(const QItemSelection &, const QItemSelection &))
                );
}

ViewClone::~ViewClone()
{
    delete ui;
}

void ViewClone::initTable()
{
    if(!m_init) {
        m_init = true;
        QStandardItemModel * itemModel = new QStandardItemModel(0, 11, this);
        ui -> tableView -> setSelectionBehavior(QAbstractItemView::SelectRows);
        ui -> tableView -> setWindowTitle("Import clone");
        ui -> tableView -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);
        ui -> tableView -> verticalHeader() -> setDefaultAlignment(Qt::AlignVCenter | Qt::AlignRight);
        ui -> tableView -> horizontalHeader() -> setStretchLastSection(true);
        ui -> tableView -> resizeColumnsToContents();
        ui -> tableView -> verticalHeader() -> setSectionsMovable(true);
        model = itemModel;
        int col = 0;
        model -> setHorizontalHeaderItem(col++, new QStandardItem(QString("UID")));
        model -> setHorizontalHeaderItem(col++, new QStandardItem(QString("Pass")));
        model -> setHorizontalHeaderItem(col++, new QStandardItem(QString("2Fa")));
        model -> setHorizontalHeaderItem(col++, new QStandardItem(QString("Token")));
        model -> setHorizontalHeaderItem(col++, new QStandardItem(QString("Ngày sinh")));
        model -> setHorizontalHeaderItem(col++, new QStandardItem(QString("Email")));
        model -> setHorizontalHeaderItem(col++, new QStandardItem(QString("Pass email")));
        model -> setHorizontalHeaderItem(col++, new QStandardItem(QString("Name Acc")));
        model -> setHorizontalHeaderItem(col++, new QStandardItem(QString("Ip")));
        model -> setHorizontalHeaderItem(col++, new QStandardItem(QString("Ghi Chú")));
        model -> setHorizontalHeaderItem(col++, new QStandardItem(QString("Trạng thái")));
        ui -> tableView -> setModel(model);
    }
}

void ViewClone::reloadTable()
{
    QBrush brush;
    QList <CommonInforClone *> listClone = SQLiteWorker::getInstance()->getListClone();
    model->removeRows(0,model->rowCount());
    for(int row = 0; row < listClone.size(); row++)
    {
        QStringList cloneInfo;
        // uid
        cloneInfo.append(listClone[row]->getUID());
        // password
        cloneInfo.append(listClone[row]->getPassword());
        // 2fa
        cloneInfo.append(listClone[row]->get2Fa());
        // token
        cloneInfo.append(listClone[row]->getToken());
        // ngay sinh
        cloneInfo.append(listClone[row]->getBirthday());
        // email
        cloneInfo.append(listClone[row]->getMail());
        // pass mail
        cloneInfo.append(listClone[row]->getPassMail());
        // name account
        cloneInfo.append(listClone[row]->getNameAcc());
        // ip
        cloneInfo.append(listClone[row]->getProxy());
        // ghi chu
        if(listClone[row]->getCheckPoint() && listClone[row]->getSttUncheckPoint() == STATUS_UNCHECKPOINT::NOMAL)
        {
            cloneInfo.append("checkpoint");
            brush = QBrush(QColor("#fff2cc"));
            brush.setStyle(Qt::SolidPattern);
        } else if(listClone[row]->getCheckPoint() && listClone[row]->getSttUncheckPoint() == STATUS_UNCHECKPOINT::WAITING) {
            cloneInfo.append("waiting");
            brush = QBrush(QColor("#915ec2"));
            brush.setStyle(Qt::SolidPattern);
        } else if(listClone[row]->getCheckPoint() && listClone[row]->getSttUncheckPoint() == STATUS_UNCHECKPOINT::UNSUCCESS) {
            cloneInfo.append("Reject");
            brush = QBrush(QColor("#e26056"));
            brush.setStyle(Qt::SolidPattern);
        } else if(listClone[row]->getCheckPoint() && listClone[row]->getSttUncheckPoint() == STATUS_UNCHECKPOINT::SUCCESS) {
            cloneInfo.append("UncheckPoint Success");
            brush = QBrush(QColor("#81FF82"));
            brush.setStyle(Qt::SolidPattern);
        } else {
            cloneInfo.append("live");
            brush = QBrush(QColor("#81FF82"));
            brush.setStyle(Qt::SolidPattern);
        }
        // trang thai
        cloneInfo.append(listClone[row]->getLastLogWithoutDate());
        int col =0;
        for(int i =0; i<cloneInfo.size(); i++)
        {
            model -> setItem(row, col, new QStandardItem(cloneInfo[i]));
            model->setData(model->index(row,col),brush,Qt::BackgroundRole);
            //model->setData(model->index(row,col),brush,Qt::ForegroundRole);
            col++;
        }
    }
}

void ViewClone::onChangeReloadTable(QString cloneId)
{
    QBrush brush;
    QList <CommonInforClone *> listClone = SQLiteWorker::getInstance()->getListClone();
    int index = 0;
    for (int i = 0; i < listClone.size(); i++) {
        if(cloneId == listClone[i]->getCloneId()) {
            index = i;
            break;
        }
    }
//    model->removeRow(index);
    QStringList cloneInfo;
    // uid
    cloneInfo.append(listClone[index]->getUID());
    // password
    cloneInfo.append(listClone[index]->getPassword());
    // 2fa
    cloneInfo.append(listClone[index]->get2Fa());
    // token
    cloneInfo.append(listClone[index]->getToken());
    // ngay sinh
    cloneInfo.append(listClone[index]->getBirthday());
    // email
    cloneInfo.append(listClone[index]->getMail());
    // pass mail
    cloneInfo.append(listClone[index]->getPassMail());
    // name account
    cloneInfo.append(listClone[index]->getNameAcc());
    // ip
    cloneInfo.append(listClone[index]->getProxy());
    // ghi chu
    if(listClone[index]->getCheckPoint() && listClone[index]->getSttUncheckPoint() == STATUS_UNCHECKPOINT::NOMAL)
    {
        cloneInfo.append("checkpoint");
        brush = QBrush(QColor("#fff2cc"));
        brush.setStyle(Qt::SolidPattern);
    } else if(listClone[index]->getCheckPoint() && listClone[index]->getSttUncheckPoint() == STATUS_UNCHECKPOINT::WAITING) {
        cloneInfo.append("waiting");
        brush = QBrush(QColor("#915ec2"));
        brush.setStyle(Qt::SolidPattern);
    } else if(listClone[index]->getCheckPoint() && listClone[index]->getSttUncheckPoint() == STATUS_UNCHECKPOINT::UNSUCCESS) {
        cloneInfo.append("Reject");
        brush = QBrush(QColor("#e26056"));
        brush.setStyle(Qt::SolidPattern);
    } else if(listClone[index]->getCheckPoint() && listClone[index]->getSttUncheckPoint() == STATUS_UNCHECKPOINT::SUCCESS) {
        cloneInfo.append("UncheckPoint Success");
        brush = QBrush(QColor("#81FF82"));
        brush.setStyle(Qt::SolidPattern);
    } else {
        cloneInfo.append("live");
        brush = QBrush(QColor("#81FF82"));
        brush.setStyle(Qt::SolidPattern);
    }
    // trang thai
    cloneInfo.append(listClone[index]->getLastLogWithoutDate());
    int col =0;
    for(int i =0; i<cloneInfo.size(); i++)
    {
        model -> setItem(index, col, new QStandardItem(cloneInfo[i]));
        model->setData(model->index(index,col),brush,Qt::BackgroundRole);
        col++;
    }

}

void ViewClone::slotSelectionChange(const QItemSelection &x,const QItemSelection &y)
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if(selectedRows.isEmpty()) {
        return;
    }
    int _ordinalNumberOfClone = selectedRows.at(0).row();
    if(listSetectedClone.empty())
    {
        listSetectedClone.append(_ordinalNumberOfClone);
        emit rowSelected(_ordinalNumberOfClone);
    }
    else
    {
        bool check = false;
        for (int i = 0; i<listSetectedClone.size(); i++)
        {
            if(_ordinalNumberOfClone == listSetectedClone.at(i))
            {
                check = true;
            }
        }
        if(check != true)
        {
            listSetectedClone.append(_ordinalNumberOfClone);
            emit rowSelected(_ordinalNumberOfClone);
        }
    }
}

void ViewClone::removedSelectionStatusSubwindow(int ordinalNumberOfClone)
{
    for (int i = 0; i<listSetectedClone.size(); i++)
    {
        if(ordinalNumberOfClone == listSetectedClone.at(i))
        {
            listSetectedClone.removeAt(i);

        }
    }
    ui->tableView->clearSelection();
}
