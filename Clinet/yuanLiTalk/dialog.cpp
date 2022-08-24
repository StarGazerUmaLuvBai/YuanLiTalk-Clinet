#include "dialog.h"
#include "ui_dialog.h"
#include "tcpnetwork.h"
#include "filestorage.h"
#include "password_change.h"
extern tcpnetwork *socket;
extern QString token;
extern QString uuid;
extern QJsonObject remembered_user;

extern QJsonArray friend_list;
extern QJsonArray group_list;

extern QJsonArray pending_friend_list;
extern QJsonArray pending_group_list;
extern int current_uid;
extern QString current_username;
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    is_token_mod=false;
    QDir dataDir("userData");
    if(dataDir.exists("lastLogin")){
        QFile file(dataDir.filePath("lastLogin"));
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray arr = file.readAll();
        qDebug()<<arr;
        file.close();
        QJsonObject lastLogin = QJsonDocument::fromJson(arr).object();
        ui->lineEdit->setText(QString::number(lastLogin["uid"].toInt()));


        if(lastLogin["rememberPassword"].toBool()){
            ui->lineEdit_2->setText("123456");
            token=lastLogin["token"].toString();
            ui->rememberPassword->setChecked(true);
            is_token_mod=true;
        }
    }
    connect(ui->lineEdit_2,QLineEdit::textEdited,[=](){
        is_token_mod=false;
    });
    connect(ui->lineEdit,QLineEdit::textEdited,[=](){
        QString uid1 = ui->lineEdit->text();
        if(remembered_user.find(uid1)!=remembered_user.end()&&remembered_user[uid1].toObject()["rememberPassword"].toBool()){
            token=remembered_user[uid1].toObject()["token"].toString();
            ui->lineEdit_2->setText("123456");
        }
        else{
            ui->lineEdit_2->setText("");
        }
    });
    connect(socket,tcpnetwork::loginResult,[=](QJsonObject &response){
        qDebug()<<"aaa";
        if(response["status"]==YUANLITALK_SYSTEM_ERROR){
            QMessageBox::warning(this,"系统错误","服务器发生了错误，请重试");
            return;
        }
        else if(response["status"]==YUANLITALK_FAILURE){
            if(is_token_mod){
                QMessageBox::warning(this,"错误","登录信息过期，请手动输入密码");
            }
            else{
                QMessageBox::warning(this,"错误","用户名或密码错误，请重试");
            }
            return;
        }
        QMessageBox::information(this,"成功","登录成功");
        if(response.find("token")!=response.end()){
            token=response["token"].toString();
            if(ui->rememberPassword->isChecked()){
                QJsonObject user_info;
                user_info["rememberPassword"]=true;
                user_info["token"]=token;
                remembered_user[QString::number(uid)]=user_info;

                QFile remembered_user_file(dataDir.filePath("rememberedUser"));
                remembered_user_file.open(QIODevice::WriteOnly | QIODevice::Text);
                remembered_user_file.write(QJsonDocument(remembered_user).toJson(QJsonDocument::Compact));
                remembered_user_file.close();

            }
            QFile lastLogin_file(dataDir.filePath("lastLogin"));
            lastLogin_file.open(QIODevice::WriteOnly | QIODevice::Text);
            QJsonObject user_info;
            user_info["uid"]=uid;
            if(ui->rememberPassword->isChecked()){
                user_info["rememberPassword"]=true;
                user_info["token"]=token;
            }
            else{
                user_info["rememberPassword"]=false;
            }

            lastLogin_file.write(QJsonDocument(user_info).toJson(QJsonDocument::Compact));
            lastLogin_file.close();
        }
        // 处理离线是收到的消息

        QJsonObject offlineMessage= response["offlineMessage"].toObject();
        QStringList offlineMessage_uid = offlineMessage.keys();
        for(QString friend_uid:offlineMessage_uid){
            update_message(uid,friend_uid.toInt(),offlineMessage[friend_uid].toArray());
        }

        QJsonObject offlineGroupMessage = response["offlineGroupMessage"].toObject();


        QStringList offlineGroupMessage_gid = offlineGroupMessage.keys();
        for(QString gid:offlineGroupMessage_gid){
            update_group_message(uid,gid.toInt(),offlineGroupMessage[gid].toArray());
        }

        // 处理好友和群聊列表

        friend_list=response["friendList"].toArray();
        group_list=response["groupList"].toArray();
        pending_friend_list=response["pendingFriendList"].toArray();
        pending_group_list=response["pendingGroupList"].toArray();
        current_uid=uid;
        current_username=response["username"].toString();
        this->hide();
        MainWindow * mw=new MainWindow;
        mw->show();
    });
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    uid = ui->lineEdit->text().toInt();
    passWord = ui->lineEdit_2->text();
    //判断长度
    if( ui->lineEdit->text().length() == 0 ){
        QMessageBox::warning(this,"输入错误","请输入用户名");
    }
    else if( passWord.length() == 0 ){
        QMessageBox::warning(this,"输入错误","请输入密码");
    }
    else {
        QJsonObject request;
        request["operation"]="login";
        request["uid"]=uid;
        if(is_token_mod){
            request["token"]=token;
        }
        else{
            request["password"]=passWord;
        }
        request["uuid"]=uuid;
        socket->YuanliTalkSend(request);
    }
}


void Dialog::on_pushButton_2_clicked()
{
    Register* reg = new Register;
    reg->show();
    this->close();
}

void Dialog::on_pushButton_3_clicked()
{
    password_change* pscg = new password_change();
    pscg->show();
    this->close();
}
