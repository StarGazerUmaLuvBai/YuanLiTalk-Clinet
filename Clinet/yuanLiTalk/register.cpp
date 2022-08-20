#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    clinet = new QTcpSocket();
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
        clinet->connectToHost("173.82.246.214",10086);  //173.82.246.214
        connect(clinet,SIGNAL(connected()),this,SLOT(hadConnected()));
    }
}

void Register::on_pushButton_2_clicked()
{
    Dialog * login = new Dialog;
    login->show();
    this->hide();
}

void Register::hadConnected(){
    QString uname = ui->lineEdit->text();
    QString passWord = ui->lineEdit_2->text();
    /*
    {
      “operation”:”注册”,
      “userName”:””,
      “password1”:””,
      “password2”:””
    }
    */
    QString data =
            "{\"operation\" : \"register\", \"username\" : \"" +
            uname + "\",\"password\" : \"" +
            passWord + "\"}";
    clinet->write(data.toUtf8());
    connect(clinet,SIGNAL(readyRead()),this,SLOT(hadReadyRead()));
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
        /*if( jsonObject["operation"].toString() != "register" ){
            QMessageBox::warning(this,"服务器信息错误","注册失败，请重试");
            return ;
        }*/
        //else {
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
        //}
    }
}
