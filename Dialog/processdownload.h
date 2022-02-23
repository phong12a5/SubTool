#ifndef PROCESSDOWNLOAD_H
#define PROCESSDOWNLOAD_H

#include <QDialog>

namespace Ui {
class ProcessDownload;
}

class ProcessDownload : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessDownload(QWidget *parent = nullptr);
    ~ProcessDownload();

public slots:
    bool setValue(int value);
    void setTitle(QString title);

private:
    Ui::ProcessDownload *ui;
};

#endif // PROCESSDOWNLOAD_H
