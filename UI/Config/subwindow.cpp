#include "subwindow.h"
#include "ui_subwindow.h"

SubWindow::SubWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SubWindow)
{
    ui->setupUi(this);
}

SubWindow::~SubWindow()
{
    delete ui;
}

void SubWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    emit closeSubWindow();
}
