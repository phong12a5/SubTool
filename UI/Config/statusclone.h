#ifndef STATUSCLONE_H
#define STATUSCLONE_H

#include <QWidget>
#include<QStringList>
#include "UI/Config/subwindow.h"
#include <QTimer>
#include "UI/Config/viewclone.h"

namespace Ui {
class StatusClone;
}

class StatusClone : public SubWindow
{
    Q_OBJECT

public:
    explicit StatusClone(int ordinalNumberOfClone);
    ~StatusClone();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

signals:
    void closeStatusSubwindow(int ordinalNumberOfClone);

private:
    Ui::StatusClone *ui;
    int m_ordinalNumberOfClone;

public slots:
    void reloadView(QStringList listLog);
};

#endif // STATUSCLONE_H
