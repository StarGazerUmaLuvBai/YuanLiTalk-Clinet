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
        /*if(response["status"].toInt()!=YUANLITALK_SUCCESS){
            QMessageBox::warning(this,"系统错误","请重试");
        }*/

    }
}

void Register::on_pushButton_2_clicked()
{
    Dialog * login = new Dialog;
    login->show();
    this->hide();
}



void Register::hadReadyRead(){
    QByteArray recvData = clinet->readAll();
    int size = recvData.size();
    if(recvData.at(0) != '{' || recvData.at(size-1) != '}'){
        QMessageBox::warning(this,"服务器信息缺失","注册失败，请重试");
        return ;
    }
    else {
        QJsonParseError parseJsonErr;
        QJsonDocument doucument = QJsonDocument::fromJson(recvData,&parseJsonErr);
        QJsonObject jsonObject = doucument.object();
        if( jsonObject["status"].toInt() != Success ){
            QMessageBox::warning(this,"注册失败","请重新设置用户和密码");
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
            ui->lineEdit_3->clear();
            return ;
        }
        else {
            int uid = jsonObject["uid"].toInt();
            QString msg = "您的uid是:"+QString::number(uid)+"\n欢迎来到猿理Talk!";
            QMessageBox::information(this,"注册成功",msg);
            Dialog * login = new Dialog;
            login->show();
            this->hide();
        }
    }
}
