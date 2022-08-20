#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    clinet = new QTcpSocket();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    QString uname = ui->lineEdit->text();
    QString passWord = ui->lineEdit_2->text();
    //判断长度
    if( uname.length() == 0 ){
        QMessageBox::warning(this,"输入错误","请输入用户名");
    }
    else if( passWord.length() == 0 ){
        QMessageBox::warning(this,"输入错误","请输入密码");
    }
    else {
        clinet->connectToHost("173.82.246.214",10086);  //173.82.246.214
        connect(clinet,SIGNAL(connected()),this,SLOT(hadConnected()));
    }
}


void Dialog::on_pushButton_2_clicked()
{
    Register* reg = new Register;
    reg->show();
    this->hide();
}


void Dialog::hadConnected(){
    QString uname = ui->lineEdit->text();
    QString passWord = ui->lineEdit_2->text();
    /*
    {
      "operation":"login",
      "userName":"",
      "password":""
    }
    */
    QString data =
            "{\"operation\" : \"login\", \"username\" : \"" +
            uname + "\",\"password\" : \"" +
            passWord + "\"}";
    clinet->write(data.toUtf8());
    connect(clinet,SIGNAL(readyRead()),this,SLOT(hadReadyRead()));
}


void Dialog::hadReadyRead(){
    QByteArray recvData = clinet->readAll();
    int size = recvData.size();
    if(recvData.at(0) != '{' || recvData.at(size-1) != '}'){
        QMessageBox::warning(this,"服务器信息缺失","登录失败，请重试");
        return ;
    }
    else {
        QJsonParseError parseJsonErr;
        QJsonDocument doucument = QJsonDocument::fromJson(recvData,&parseJsonErr);
        QJsonObject jsonObject = doucument.object();
        /*if( jsonObject["operation"].toString() != "login" ){
            QMessageBox::warning(this,"服务器信息错误","登录失败，请重试");
            return ;
        }*/
        //else {
            if( jsonObject["status"].toInt() != Success ){
                QMessageBox::warning(this,"登录失败","账户或密码错误，请重试");
                return ;
            }
            else {
                QMessageBox::information(this,"登录成功","欢迎使用猿理Talk!");
                //TODO:登录成功后接MainWindow
                MainWindow *mWindow = new MainWindow(clinet);
                disconnect(clinet,0,this,0);
                mWindow->show();
                this->hide();
            }
        //}
    }
}
