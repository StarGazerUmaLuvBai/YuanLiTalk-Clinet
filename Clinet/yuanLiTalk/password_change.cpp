#include "password_change.h"
#include "ui_password_change.h"
#include "tcpnetwork.h"
extern int current_uid;
extern tcpnetwork *socket;
password_change::password_change(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::password_change)
{
    ui->setupUi(this);
    if(current_uid){
        QJsonObject req;
        req["uid"]=current_uid;
        req["operation"]="getMibao";
        socket->YuanliTalkSend(req);
    }
    connect(ui->go_to_nxt,QPushButton::clicked,[=](){
        QJsonObject req;
        current_uid=ui->uid->text().toInt();
        req["uid"]=ui->uid->text().toInt();
        req["operation"]="getMibao";
        socket->YuanliTalkSend(req);
    });
    connect(socket,tcpnetwork::getMibaoResult,[=](const QJsonObject &resp){
        if(resp["status"].toInt()!=YUANLITALK_SUCCESS){
            QMessageBox::critical(this,"","未找到用户");
            return;
        }
        ui->mibao->setText(resp["mibao"].toString());
        ui->stackedWidget->setCurrentWidget(ui->page);
    });
    connect(ui->uid,QLineEdit::textEdited,[=](){
        if(ui->uid->text()==""){
            ui->go_to_nxt->setEnabled(false);
        }
        else{
            ui->go_to_nxt->setEnabled(true);
        }
    });
    connect(socket,tcpnetwork::changePasswordResult,[=](const QJsonObject &resp){
        if(resp["status"].toInt()!=YUANLITALK_SUCCESS){
            QMessageBox::critical(this,"","修改失败");
            return;
        }
        QMessageBox::information(this,"","修改成功");
    });

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
    QString pWord = ui->password->text();
    QString pWord_confirm = ui->password_ensure->text();
    QString mibao_answer = ui->mibao_answer->text();
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
        QJsonObject req;
        req["operation"]="changePassword";
        req["uid"]=current_uid;
        req["password"]=pWord;
        req["mibaoAnswer"]=mibao_answer;
        socket->YuanliTalkSend(req);
    }
}

