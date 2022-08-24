#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpnetwork.h"
#include <QJsonValue>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QPushButton>
#include <QDateTime>
#include <QScrollBar>
#include <QLayoutItem>
#include <QSize>
#include "filestorage.h"
#include "all.h"
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    ui->profile->setIcon(socket->profile_map[current_uid]);
    ui->profile->setIconSize(QSize(50,50));
    current_chatting_id=-1;
    ui->label_uid->setText(QString::number(current_uid));
    ui->label_username->setText(current_username);
    QVBoxLayout *l= new QVBoxLayout();
    QPushButton *p;
    l->setAlignment(Qt::AlignTop);
    for(QJsonValue item:friend_list){
        p=new QPushButton;
        p->setText(item.toObject()["username"].toString());
        p->setMinimumHeight(50);
        if(item.toObject().find("profilephoto")!=item.toObject().end()&&item.toObject()["profilephoto"].toString()!=""){
            p->setIcon(QIcon(base64_to_pixmap(item.toObject()["profilephoto"].toString().toUtf8())));
        }
        else{
            p->setIcon(QIcon(QPixmap(":/img/default_profilephoto.jpg","jpg")));
        }
        p->setIconSize(QSize(50,50));
        p->setStyleSheet("text-align:left");
        p->setFlat(true);
        int uid=item.toObject()["uid"].toInt();
        connect(p,QPushButton::clicked,[=](){
            QVBoxLayout *m= new QVBoxLayout();
            m->setAlignment(Qt::AlignTop);
            QPushButton *q;
            QJsonArray arr = get_history_message(current_uid,uid);
            for(QJsonValue item:arr){
                QJsonObject obj= item.toObject();
                q=new QPushButton;
                int senderUid=obj["senderUid"].toInt();

                q->setMinimumHeight(60);
                q->setFlat(true);
                QLabel* labIMG = new QLabel();
                QLabel* labInfo = new QLabel();
                //加载图片到Label
                labIMG->setPixmap(socket->profile_map[senderUid].scaled(50,50,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
                //设置描述按钮功能的文字
                QString strInfo = obj["message"].toString();
                labInfo->setText(strInfo);
                //设置文字的字样、字体、颜色等样式及文本框宽度
                labInfo->setStyleSheet("font: 15px 'Microsoft YaHei';");
                labInfo->setFixedWidth(200);
                //新建布局，把元素一一添加到布局，再把布局设置到按钮上
                QHBoxLayout* LayoutInfo = new QHBoxLayout();
                LayoutInfo->addSpacing(2);
                LayoutInfo->addWidget(labIMG);
                LayoutInfo->addSpacing(10);
                LayoutInfo->addWidget(labInfo);
                LayoutInfo->addStretch();
                q->setLayout(LayoutInfo);
                m->addWidget(q);

            }
            if(ui->message_scroll->layout()){
                QLayout *layout=ui->message_scroll->layout();
                QLayoutItem *child;
                while ((child = layout->takeAt(0)) != 0) {
                    delete child->widget();
                }

                delete ui->message_scroll->layout();
            }
            ui->message_scroll->setLayout(m);
            current_chatting_type="friend";
            current_chatting_id=uid;
        });
        l->addWidget(p);
    }
    ui->friends_scroll->setLayout(l);


    l= new QVBoxLayout();
    l->setAlignment(Qt::AlignTop);

    for(QJsonValue item:group_list){

        p=new QPushButton;
        p->setText(item.toObject()["groupname"].toString());
        p->setMinimumHeight(50);
        if(item.toObject().find("profilephoto")!=item.toObject().end()&&item.toObject()["profilephoto"].toString()!=""){
            p->setIcon(QIcon(base64_to_pixmap(item.toObject()["profilephoto"].toString().toUtf8())));
        }
        else{
            p->setIcon(QIcon(QPixmap(":/img/default_profilephoto.jpg","jpg")));
        }
        p->setIconSize(QSize(50,50));
        p->setStyleSheet("text-align:left");
        p->setFlat(true);
        int gid=item.toObject()["gid"].toInt();
        connect(p,QPushButton::clicked,[=](){
            QVBoxLayout *m= new QVBoxLayout();
            m->setAlignment(Qt::AlignTop);
            QPushButton *q;
            QJsonArray arr = get_history_group_message(current_uid,gid);
            for(QJsonValue item:arr){
                QJsonObject obj= item.toObject();
                q=new QPushButton;
                int senderUid=obj["senderUid"].toInt();

                q->setMinimumHeight(60);
                q->setFlat(true);
                QLabel* labIMG = new QLabel();
                QLabel* labInfo = new QLabel();
                //加载图片到Label
                labIMG->setPixmap(socket->profile_map[senderUid].scaled(50,50,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
                //设置描述按钮功能的文字
                QString strInfo = obj["message"].toString();
                labInfo->setText(strInfo);
                //设置文字的字样、字体、颜色等样式及文本框宽度
                labInfo->setStyleSheet("font: 15px 'Microsoft YaHei';");
                labInfo->setFixedWidth(200);
                //新建布局，把元素一一添加到布局，再把布局设置到按钮上
                QHBoxLayout* LayoutInfo = new QHBoxLayout();
                LayoutInfo->addSpacing(2);
                LayoutInfo->addWidget(labIMG);
                LayoutInfo->addSpacing(10);
                LayoutInfo->addWidget(labInfo);
                LayoutInfo->addStretch();
                q->setLayout(LayoutInfo);
                m->addWidget(q);
            }
            if(ui->message_scroll->layout()){
                QLayout *layout=ui->message_scroll->layout();
                QLayoutItem *child;
                while ((child = layout->takeAt(0)) != 0) {
                    delete child->widget();
                }

                delete ui->message_scroll->layout();
            }
            ui->message_scroll->setLayout(m);
            current_chatting_type="group";
            current_chatting_id=gid;
        });
        l->addWidget(p);
    }
    ui->group_scroll->setLayout(l);


    l= new QVBoxLayout();
    l->setAlignment(Qt::AlignTop);
    for(QJsonValue item:pending_friend_list){
        p=new QPushButton;
        p->setText(item.toObject()["username"].toString());
        p->setMinimumHeight(50);
        if(item.toObject().find("profilephoto")!=item.toObject().end()&&item.toObject()["profilephoto"].toString()!=""){
            p->setIcon(QIcon(base64_to_pixmap(item.toObject()["profilephoto"].toString().toUtf8())));
        }
        else{
            p->setIcon(QIcon(QPixmap(":/img/default_profilephoto.jpg","jpg")));
        }
        p->setIconSize(QSize(50,50));
        p->setStyleSheet("text-align:left");
        p->setFlat(true);
        int uid=item.toObject()["uid"].toInt();
        connect(p,QPushButton::clicked,[=](){
            int res=QMessageBox::question(this,"","同意加为好友？");
            QJsonObject req;
            req["operation"]="accepetAddUser";
            req["uid"]=current_uid;
            req["token"]=token;
            req["uuid"]=uuid;
            req["fromWho"]=uid;
            req["accept"]=res==QMessageBox::Yes;

            socket->YuanliTalkSend(req);
            delete p;
        });
        l->addWidget(p);
    }
    ui->friend_requests->setLayout(l);


    l= new QVBoxLayout();
    l->setAlignment(Qt::AlignTop);
    for(QJsonValue item:pending_group_list){
        p=new QPushButton;
        p->setText(item.toObject()["username"].toString()+"申请进入群聊"+QString::number( item.toObject()["gid"].toInt()));
        p->setMinimumHeight(50);
        if(item.toObject().find("profilephoto")!=item.toObject().end()&&item.toObject()["profilephoto"].toString()!=""){
            p->setIcon(QIcon(base64_to_pixmap(item.toObject()["profilephoto"].toString().toUtf8())));
        }
        else{
            p->setIcon(QIcon(QPixmap(":/img/default_profilephoto.jpg","jpg")));
        }
        p->setIconSize(QSize(50,50));
        p->setStyleSheet("text-align:left");
        p->setFlat(true);
        int uid=item.toObject()["uid"].toInt();
        int gid=item.toObject()["gid"].toInt();
        connect(p,QPushButton::clicked,[=](){
            int res=QMessageBox::question(this,"","同意加入群聊？");
            QJsonObject req;
            req["operation"]="accepetAddGroup";
            req["uid"]=current_uid;
            req["token"]=token;
            req["uuid"]=uuid;
            req["fromWho"]=uid;
            req["gid"]=gid;
            req["accept"]=res==QMessageBox::Yes;

            socket->YuanliTalkSend(req);
            delete p;
        });
        l->addWidget(p);
    }
    ui->group_requests->setLayout(l);


    connect(socket,tcpnetwork::getMessage,[=](const QJsonObject &resp){

        QJsonObject msg;
        msg["senderUid"]=resp["senderUid"];
        msg["sendingTime"]=resp["sendingTime"];
        msg["message"]=resp["message"];
        msg["type"]=resp["type"];
        msg["receiverUid"]=current_uid;
        QJsonArray msg_arr;
        msg_arr.push_back(msg);
        update_message(current_uid,msg["senderUid"].toInt(),msg_arr);
        if(current_chatting_type=="friend"&&current_chatting_id==resp["senderUid"].toInt()){
            QPushButton *q=new QPushButton;

            q->setMinimumHeight(60);
            q->setFlat(true);
            QLabel* labIMG = new QLabel();
            QLabel* labInfo = new QLabel();
            //加载图片到Label
            labIMG->setPixmap(socket->profile_map[resp["senderUid"].toInt()].scaled(50,50,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
            //设置描述按钮功能的文字
            QString strInfo = resp["message"].toString();
            labInfo->setText(strInfo);
            //设置文字的字样、字体、颜色等样式及文本框宽度
            labInfo->setStyleSheet("font: 15px 'Microsoft YaHei';");
            labInfo->setFixedWidth(200);
            //新建布局，把元素一一添加到布局，再把布局设置到按钮上
            QHBoxLayout* LayoutInfo = new QHBoxLayout();
            LayoutInfo->addSpacing(2);
            LayoutInfo->addWidget(labIMG);
            LayoutInfo->addSpacing(10);
            LayoutInfo->addWidget(labInfo);
            LayoutInfo->addStretch();
            q->setLayout(LayoutInfo);

            ui->message_scroll->layout()->addWidget(q);
        }
    });

    connect(socket,tcpnetwork::getGroupMessage,[=](const QJsonObject &resp){
        QJsonObject msg;
        msg["senderUid"]=resp["senderUid"];
        msg["sendingTime"]=resp["sendingTime"];
        msg["message"]=resp["message"];
        msg["type"]=resp["type"];
        msg["gid"]=resp["gid"];
        QJsonArray msg_arr;
        msg_arr.push_back(msg);
        update_group_message(current_uid,msg["gid"].toInt(),msg_arr);
        if(current_chatting_type=="group"&&current_chatting_id==resp["gid"].toInt()){
            QPushButton *q=new QPushButton;

            q->setMinimumHeight(60);
            q->setFlat(true);
            QLabel* labIMG = new QLabel();
            QLabel* labInfo = new QLabel();
            //加载图片到Label
            labIMG->setPixmap(socket->profile_map[resp["senderUid"].toInt()].scaled(50,50,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
            //设置描述按钮功能的文字
            QString strInfo = resp["message"].toString();
            labInfo->setText(strInfo);
            //设置文字的字样、字体、颜色等样式及文本框宽度
            labInfo->setStyleSheet("font: 15px 'Microsoft YaHei';");
            labInfo->setFixedWidth(200);
            //新建布局，把元素一一添加到布局，再把布局设置到按钮上
            QHBoxLayout* LayoutInfo = new QHBoxLayout();
            LayoutInfo->addSpacing(2);
            LayoutInfo->addWidget(labIMG);
            LayoutInfo->addSpacing(10);
            LayoutInfo->addWidget(labInfo);
            LayoutInfo->addStretch();
            q->setLayout(LayoutInfo);

            ui->message_scroll->layout()->addWidget(q);
        }
    });

    connect(ui->sendMsg,QPushButton::clicked,[=](){
        if(ui->textEdit->toPlainText()==""||current_chatting_id==-1){
            return;
        }
        if(current_chatting_type=="friend"){
            QJsonObject msg;
            QJsonObject req;
            msg["senderUid"]=req["senderUid"]=current_uid;
            msg["sendingTime"]=(long long)QDateTime::currentSecsSinceEpoch();
            msg["message"]=req["message"]=ui->textEdit->toPlainText();
            msg["type"]=req["type"]="text";
            msg["receiverUid"]=req["receiverUid"]=current_chatting_id;
            req["uuid"]=uuid;
            req["token"]=token;
            req["operation"]="sendMessage";
            socket->YuanliTalkSend(req);
            QJsonArray msg_arr;
            msg_arr.push_back(msg);
            update_message(current_uid,current_chatting_id,msg_arr);

            QPushButton *q=new QPushButton;
            q->setText(QString::number(current_uid)+":"+req["message"].toString());
            ui->message_scroll->layout()->addWidget(q);


        }
        else{
            QJsonObject msg;
            QJsonObject req;
            msg["senderUid"]=req["senderUid"]=current_uid;
            msg["sendingTime"]=(long long)QDateTime::currentSecsSinceEpoch();
            msg["message"]=req["message"]=ui->textEdit->toPlainText();
            msg["type"]=req["type"]="text";
            msg["gid"]=req["gid"]=current_chatting_id;
            req["uuid"]=uuid;
            req["token"]=token;
            req["operation"]="sendGroupMessage";
            socket->YuanliTalkSend(req);
            QJsonArray msg_arr;
            msg_arr.push_back(msg);
            update_message(current_uid,current_chatting_id,msg_arr);

            QPushButton *q=new QPushButton;
            q->setText(QString::number(current_uid)+":"+req["message"].toString());
            ui->message_scroll->layout()->addWidget(q);
        }
        ui->textEdit->setText("");
    });
    connect(ui->Messages->verticalScrollBar(),QScrollBar::rangeChanged,[=](){
        int nTotalValue = ui->Messages->verticalScrollBar()->maximum();
        ui->Messages->verticalScrollBar()->setValue(nTotalValue);
    });

    connect(ui->search_friend_key,QLineEdit::textChanged,[=](){
        if(ui->search_friend_key->text()==""){
            ui->btn_search->setEnabled(false);
        }
        else{
            ui->btn_search->setEnabled(true);
        }
    });
    connect(ui->btn_search,QPushButton::clicked,[=](){
        QJsonObject j;
        j["operation"]="searchUser";
        j["uid"]=current_uid;
        j["uuid"]=uuid;
        j["token"]=token;
        j["key"]=ui->search_friend_key->text();
        socket->YuanliTalkSend(j);
    });

    connect(socket,tcpnetwork::searchUserResult,[=](const QJsonObject &resp){
        if(resp["status"].toInt()!=YUANLITALK_SUCCESS){
            QMessageBox::warning(this,"错误","系统错误");
            return;
        }
        QJsonArray user_list=resp["list"].toArray();
        qDebug()<<QJsonDocument( user_list).toJson()<<'\n';
        QVBoxLayout *l=new QVBoxLayout;
        l->setAlignment(Qt::AlignTop);
        QPushButton *p;
        for(QJsonValue item:user_list){
            QJsonObject obj=item.toObject();
            int uid=obj["uid"].toInt();
            QString username=obj["username"].toString();

            p=new QPushButton;
            p->setText(username+"\n"+QString::number(uid));
            if(item.toObject().find("profilephoto")!=item.toObject().end()&&item.toObject()["profilephoto"].toString()!=""){
                p->setIcon(QIcon(base64_to_pixmap(item.toObject()["profilephoto"].toString().toUtf8())));
            }
            else{
                p->setIcon(QIcon(QPixmap(":/img/default_profilephoto.jpg","jpg")));
            }
            p->setIconSize(QSize(50,50));
            p->setStyleSheet("text-align:left");
            p->setFlat(true);
            p->setMinimumHeight(50);
            l->addWidget(p);

            connect(p,QPushButton::clicked,[=](){
                if(QMessageBox::question(this,"","申请好友？")==QMessageBox::Yes){
                    QJsonObject req;
                    req["operation"]="addUser";
                    req["uid"]=current_uid;
                    req["token"]=token;
                    req["uuid"]=uuid;
                    req["addUid"]=uid;
                    socket->YuanliTalkSend(req);
                }

            });
        }

        if(ui->search_scroll->layout()){
            QLayout *layout=ui->search_scroll->layout();
            QLayoutItem *child;
            while ((child = layout->takeAt(0)) != 0) {
                delete child->widget();
            }
            delete ui->search_scroll->layout();
        }
        ui->search_scroll->setLayout(l);
    });

    connect(ui->search_group_key,QLineEdit::textChanged,[=](){
        if(ui->search_group_key->text()==""){
            ui->btn_group_search->setEnabled(false);
        }
        else{
            ui->btn_group_search->setEnabled(true);
        }
    });
    connect(ui->btn_group_search,QPushButton::clicked,[=](){
        QJsonObject j;
        j["operation"]="searchGroup";
        j["uid"]=current_uid;
        j["uuid"]=uuid;
        j["token"]=token;
        j["key"]=ui->search_group_key->text();
        socket->YuanliTalkSend(j);
    });

    connect(socket,tcpnetwork::searchGroupResult,[=](const QJsonObject &resp){
        if(resp["status"].toInt()!=YUANLITALK_SUCCESS){
            QMessageBox::warning(this,"错误","系统错误");
            return;
        }
        QJsonArray group_list=resp["list"].toArray();
        qDebug()<<QJsonDocument( group_list).toJson()<<'\n';
        QVBoxLayout *l=new QVBoxLayout;
        l->setAlignment(Qt::AlignTop);
        QPushButton *p;
        for(QJsonValue item:group_list){
            QJsonObject obj=item.toObject();
            int gid=obj["gid"].toInt();
            QString groupname=obj["groupname"].toString();


            p=new QPushButton;
            p->setText(groupname+"\n"+QString::number(gid));
            if(item.toObject().find("profilephoto")!=item.toObject().end()&&item.toObject()["profilephoto"].toString()!=""){
                p->setIcon(QIcon(base64_to_pixmap(item.toObject()["profilephoto"].toString().toUtf8())));
            }
            else{
                p->setIcon(QIcon(QPixmap(":/img/default_profilephoto.jpg","jpg")));
            }
            p->setIconSize(QSize(50,50));
            p->setStyleSheet("text-align:left");
            p->setFlat(true);
            p->setMinimumHeight(50);
            l->addWidget(p);


            connect(p,QPushButton::clicked,[=](){
                if(QMessageBox::question(this,"","申请加入该群？")==QMessageBox::Yes){
                    QJsonObject req;
                    req["operation"]="addGroup";
                    req["uid"]=current_uid;
                    req["token"]=token;
                    req["uuid"]=uuid;
                    req["addGid"]=gid;
                    socket->YuanliTalkSend(req);
                }
            });
        }

        if(ui->search_group_scroll->layout()){
            QLayout *layout=ui->search_group_scroll->layout();
            QLayoutItem *child;
            while ((child = layout->takeAt(0)) != 0) {
                delete child->widget();
            }
            delete ui->search_group_scroll->layout();
        }
        ui->search_group_scroll->setLayout(l);
    });
    connect(socket,tcpnetwork::addGroupResult,[=](const QJsonObject &resp){
        if(resp["status"].toInt()==YUANLITALK_ALREADY_EXIST){
            QMessageBox::warning(this,"",resp["info"].toString());
        }
    });
    connect(socket,tcpnetwork::addUserResult,[=](const QJsonObject &resp){
        if(resp["status"].toInt()==YUANLITALK_ALREADY_EXIST){
            QMessageBox::warning(this,"",resp["info"].toString());
        }
    });



}

MainWindow::~MainWindow()
{
    delete ui;
}
