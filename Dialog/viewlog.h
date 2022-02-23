#ifndef VIEWLOG_H
#define VIEWLOG_H

#include <QDialog>

namespace Ui {
class ViewLog;
}

class ViewLog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewLog(QWidget *parent = nullptr);
    ~ViewLog();

private:
    Ui::ViewLog *ui;
};

#endif // VIEWLOG_H
