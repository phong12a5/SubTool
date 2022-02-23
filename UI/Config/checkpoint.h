#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <QWidget>
#include "subwindow.h"

namespace Ui {
class CheckPoint;
}

class CheckPoint : public SubWindow
{
    Q_OBJECT

public:
    explicit CheckPoint(QWidget *parent = nullptr);
    ~CheckPoint();

    QString getApiToken();
    QString getApiPhone();
    QString getApiEmail();
private:
    Ui::CheckPoint *ui;
};

#endif // CHECKPOINT_H
