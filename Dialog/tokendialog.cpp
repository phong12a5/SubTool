#include "Dialog/tokendialog.h"
#include "ui_tokendialog.h"


TokenDialog::TokenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TokenDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(0);
}

TokenDialog::~TokenDialog()
{
    delete ui;
}

// click button OK
void TokenDialog::on_buttonBox_accepted()
{
    emit token(ui->token_edit->text(), true);
}

void TokenDialog::on_buttonBox_rejected()
{
    emit token(ui->token_edit->text(), false);
}

void TokenDialog::on_token_edit_textEdited(const QString &arg1)
{
    if(ui->token_edit->text().length() > 8)
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(1);
    }
    else
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(0);
    }
}

