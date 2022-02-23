#ifndef INTERACTCONFIG_H
#define INTERACTCONFIG_H

#include "subwindow.h"
#include "definition.h"
#include "sqliteworker.h"
#include <QCloseEvent>
#include <QFileDialog>

namespace Ui {
class InteractConfig;
}

class InteractConfig : public SubWindow
{
    Q_OBJECT

public:
    explicit InteractConfig(QWidget *parent = nullptr);
    ~InteractConfig();

    BaseConfig *getBaseConfig();
    Thread *getThread();

private slots:
    void on_pushButton_clicked();

private:
    Ui::InteractConfig *ui;

    BaseConfig baseConfig;
    Thread thread;
    InteractNewFeed interactNewFeed;
    InteractWatch interactWatch;

    void updateBaseCofig();
    void updateThread();
};

#endif // INTERACTCONFIG_H
