#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class SubWindow;
}

class SubWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SubWindow(QWidget *parent = nullptr);
    virtual ~SubWindow();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

signals:
    void closeSubWindow();

protected:
    Ui::SubWindow *ui;
};

#endif // SUBWINDOW_H
