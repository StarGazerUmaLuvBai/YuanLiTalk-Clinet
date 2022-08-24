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
#include "filestorage.h"
#include "all.h"
extern tcpnetwork *socket;
extern QString token;
extern QString uuid;
extern QJsonObject remembered_user;

extern QJsonArray friend_list;
extern QJsonArray group_list;

extern int current_uid;
extern QString current_username;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    current_chatting_id=-1;
    ui->label_uid->setText(QString::number(current_uid));
    ui->label_username->setText(current_username);
    QVBoxLayout *l= new QVBoxLayout();
    QPushButton *p;
    l->setAlignment(Qt::AlignTop);
    for(QJsonValue item:friend_list){
        p=new QPushButton;
        qDebug()<<QJsonDocument(item.toObject()).toJson();
        p->setText(item.toObject()["username"].toString());
        p->setMinimumHeight(50);
        int uid=item.toObject()["uid"].toInt();
        connect(p,QPushButton::clicked,[=](){
            QVBoxLayout *m= new QVBoxLayout();
            m->setAlignment(Qt::AlignTop);
            QPushButton *q;
            QJsonArray arr = get_history_message(current_uid,uid);
            for(QJsonValue item:arr){
                QJsonObject obj= item.toObject();
                q=new QPushButton;
                q->setText(QString::number(obj["senderUid"].toInt())+":"+obj["message"].toString());
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
    qDebug()<<"glsize:"<<group_list.size();
    for(QJsonValue item:group_list){

        p=new QPushButton;
        p->setText(item.toObject()["groupname"].toString());
        p->setMinimumHeight(50);
        int gid=item.toObject()["gid"].toInt();
        connect(p,QPushButton::clicked,[=](){
            QVBoxLayout *m= new QVBoxLayout();
             m->setAlignment(Qt::AlignTop);
            QPushButton *q;
            QJsonArray arr = get_history_group_message(current_uid,gid);
            for(QJsonValue item:arr){
                QJsonObject obj= item.toObject();
                q=new QPushButton;
                q->setText(QString::number(obj["senderUid"].toInt())+":"+obj["message"].toString());
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
            q->setText(QString::number(resp["senderUid"].toInt())+":"+resp["message"].toString());
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
            q->setText(QString::number(resp["senderUid"].toInt())+":"+resp["message"].toString());
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
            p->setText("添加 "+username+"("+QString::number(uid)+") 为好友");
            p->setMinimumHeight(50);
            l->addWidget(p);

            connect(p,QPushButton::clicked,[=](){
                QJsonObject req;
                req["operation"]="addUser";
                req["uid"]=current_uid;
                req["token"]=token;
                req["uuid"]=uuid;
                req["addUid"]=uid;
                socket->YuanliTalkSend(req);
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
}

MainWindow::~MainWindow()
{
    delete ui;
}
