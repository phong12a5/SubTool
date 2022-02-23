#ifndef VIEWCLONE_H
#define VIEWCLONE_H

#include "subwindow.h"
#include <QCloseEvent>
#include <QTableView>
#include <QStandardItem>
#include <QTableWidgetItem>
#include <QModelIndex>
#include <QtSql/QSql>
#include <QFileInfo>
#include "CloneInfo.h"
#include "sqliteworker.h"

namespace Ui {
class ViewClone;
}

class ViewClone : public SubWindow
{
    Q_OBJECT

public:
    explicit ViewClone(QWidget *parent = nullptr);
    ~ViewClone();

    void initTable();
    void reloadTable();
    void onChangeReloadTable(QString cloneId);
    int getOrdinalNumberOfClone();

private slots:
    void slotSelectionChange(const QItemSelection &x,const QItemSelection &y);
public slots:
    void removedSelectionStatusSubwindow(int ordinalNumberOfClone);
private:
    Ui::ViewClone *ui;
    QStandardItemModel *model;
    bool m_init = false;
    QList<int> listSetectedClone;
signals:
    void rowSelected(int ordinalNumberOfClone);

};

#endif // VIEWCLONE_H
