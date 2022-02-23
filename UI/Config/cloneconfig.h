#ifndef CLONECONFIG_H
#define CLONECONFIG_H

#include "subwindow.h"
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QCloseEvent>

namespace Ui {
class CloneConfig;
}

class CloneConfig : public SubWindow
{
    Q_OBJECT
public:
    explicit CloneConfig(QWidget *parent = nullptr);
    ~CloneConfig();
private:
    Ui::CloneConfig *ui;
};

#endif // CLONECONFIG_H
