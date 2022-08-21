#include "password_change.h"
#include "ui_password_change.h"

password_change::password_change(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::password_change)
{
    ui->setupUi(this);
}
password_change::password_change(QTcpSocket *sock, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::password_change)
{
    ui->setupUi(this);
    clinet = sock;
}

password_change::~password_change()
{
    delete ui;
}

void password_change::on_no_clicked()
{
    close();
}

void password_change::on_yes_clicked()
{
    QString pWord = ui->pswd_first->text();
    QString pWord_confirm = ui->pswd_last->text();
    if(pWord.length() == 0)
    {
        QMessageBox::warning(this,"输入错误","请输入密码");
    }
    else if(pWord != pWord_confirm)
    {
        QMessageBox::warning(this,"输入错误","两次密码不一致");
    }
    else
    {
        QString packData = "{\"operation\" : \"passwordChange\" , \"password\" : "+pWord+"\"}";
        clinet->write(packData.toUtf8());
    }
}

void password_change::RecStatus(int status)
{
    if(status == 1)
    {
        QMessageBox::warning(this,"失败","修改失败，请稍后再试");
    }
    else
    {
        QMessageBox::information(this,"成功","修改成功");
    }
    this->hide();
}
