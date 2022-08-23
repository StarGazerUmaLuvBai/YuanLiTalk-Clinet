#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    connect(socket,tcpnetwork::registerResult,[=](QJsonObject response){
        if(response["status"]!=YUANLITALK_SUCCESS){
            QMessageBox::warning(this,"系统错误","服务器发生了错误，请重试");
            return;
        }
        QMessageBox::information(this,"注册成功","您的账号为："+QString::number(response["uid"].toInt()));
    });
}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_clicked()
{
    QString uname = ui->lineEdit->text();
    QString passWord = ui->lineEdit_2->text();
    QString passWord_ensure = ui->lineEdit_3->text();
    if(uname.length() == 0){
        QMessageBox::warning(this,"输入错误","请输入用户名");
    }
    else if( passWord.length() == 0){
        QMessageBox::warning(this,"输入错误","请输入密码");
    }
    else if( passWord_ensure.length() == 0){
        QMessageBox::warning(this,"输入错误","请确认密码");
    }
    else if( passWord != passWord_ensure ){
        QMessageBox::warning(this,"输入错误","两次密码输入不符");
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
    }
    else {
        QString uname = ui->lineEdit->text();
        QString passWord = ui->lineEdit_2->text();
        QJsonObject request;
        request["operation"]="register";
        request["username"]=uname;
        request["password"]=passWord;
        socket->YuanliTalkSend(request);

    }
}

void Register::on_pushButton_2_clicked()
{
    Dialog * login = new Dialog;
    login->show();
    this->hide();
}


