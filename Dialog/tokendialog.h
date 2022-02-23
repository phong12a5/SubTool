#ifndef TOKENDIALOG_H
#define TOKENDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QPushButton>

namespace Ui {
class TokenDialog;
}

class TokenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TokenDialog(QWidget *parent = nullptr);
    ~TokenDialog();
signals:
    void token(QString token, bool x);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_token_edit_textEdited(const QString &arg1);

private:
    Ui::TokenDialog *ui;

};

#endif // TOKENDIALOG_H
