#ifndef INFOVERSION_H
#define INFOVERSION_H

#include <QDialog>
#include "AppDefine.h"

namespace Ui {
class InfoVersion;
}

class InfoVersion : public QDialog
{
    Q_OBJECT

public:
    explicit InfoVersion(QWidget *parent = nullptr);
    ~InfoVersion();

private:
    Ui::InfoVersion *ui;
};

#endif // INFOVERSION_H
