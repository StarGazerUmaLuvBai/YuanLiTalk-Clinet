#include "register.h"
#include "ui_register.h"
Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    profilephoto_name="";
    connect(socket,tcpnetwork::registerResult,[=](QJsonObject response){
        if(response["status"]!=YUANLITALK_SUCCESS){
            QMessageBox::warning(this,"系统错误","服务器发生了错误，请重试");
            return;
        }
        QMessageBox::information(this,"注册成功","您的账号为："+QString::number(response["uid"].toInt()));
    });
    connect(ui->profilephoto_btn,QPushButton::clicked,[=](){
        //定义文件对话框类
        QFileDialog *fileDialog = new QFileDialog(this);

        //定义文件对话框标题
        fileDialog->setWindowTitle(QStringLiteral("选择文件"));
        fileDialog->setNameFilter("*.png *.jpg");
        //设置打开的文件路径
        fileDialog->setDirectory("./");

        //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
        fileDialog->setFileMode(QFileDialog::ExistingFiles);

        //设置视图模式
        fileDialog->setViewMode(QFileDialog::Detail);

        //获取选择的文件的路径
        QStringList fileNames;
        if (fileDialog->exec()) {
            fileNames = fileDialog->selectedFiles();
            if(fileNames.size()){
                profilephoto_name=fileNames[0];
                qDebug()<<profilephoto_name;

                QPixmap qp = QPixmap(profilephoto_name).scaled(QSize(80,80),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
                ui->profilephoto_btn->setIcon(QIcon(qp));
                ui->profilephoto_btn->setIconSize(QSize(80,80));
                ui->profilephoto_btn->setFlat(true);
                ui->profilephoto_btn->setText("");
                profilephoto_base64 = pixmap_to_base64(qp);
            }
        }
        delete fileDialog;

    });
}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_clicked()
{
    QString uname = ui->username->text();
    QString passWord = ui->password->text();
    QString passWord_ensure = ui->password_ensure->text();
    QString mibao = ui->mibao->text();
    QString mibao_answer = ui->mibao_answer->text();
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
        ui->password->clear();
        ui->password_ensure->clear();
    }
    else {
        QString uname = ui->username->text();
        QString passWord = ui->password->text();
        QJsonObject request;
        request["operation"]="register";
        request["username"]=uname;
        request["password"]=passWord;
        request["mibao"]=mibao;
        request["mibaoAnswer"]=mibao_answer;
        request["profilephoto"]=profilephoto_base64;
        socket->YuanliTalkSend(request);

    }
}

void Register::on_pushButton_2_clicked()
{
    Dialog * login = new Dialog;
    login->show();
    this->close();

}


