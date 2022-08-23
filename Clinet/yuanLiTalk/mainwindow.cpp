#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpnetwork.h"
#include <QJsonValue>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QPushButton>
#include <QDateTime>
#include "filestorage.h"
extern tcpnetwork *socket;
extern QString token;
extern QString uuid;
extern QJsonObject remembered_user;

extern QJsonArray friend_list;
extern QJsonArray group_list;

extern int current_uid;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    current_chatting_id=-1;

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
            qDebug()<<"进去了";
            QPushButton *q;
            QJsonArray arr = get_history_message(current_uid,uid);
            for(QJsonValue item:arr){
                QJsonObject obj= item.toObject();
                q=new QPushButton;
                q->setText(QString::number(obj["senderUid"].toInt())+":"+obj["message"].toString());
                m->addWidget(q);

            }
            ui->Messages->setLayout(m);
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
            ui->Messages->setLayout(m);
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
            ui->Messages->layout()->addWidget(q);
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
        if(current_chatting_type=="group"&&current_chatting_id==resp["senderUid"].toInt()){
            QPushButton *q=new QPushButton;
            q->setText(QString::number(resp["senderUid"].toInt())+":"+resp["message"].toString());
            ui->Messages->layout()->addWidget(q);
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
        }
        else{
            QJsonObject msg;
            QJsonObject req;
            msg["senderUid"]=req["senderUid"]=current_uid;
            msg["sendingTime"]=(long long)QDateTime::currentSecsSinceEpoch();
            msg["message"]=req["message"]=ui->textEdit->toPlainText();
            msg["type"]=req["type"]="text";
            msg["gid"]=current_chatting_id;
            req["uuid"]=uuid;
            req["token"]=token;
            req["operation"]="sendGroupMessage";
            socket->YuanliTalkSend(req);
            QJsonArray msg_arr;
            msg_arr.push_back(msg);
            update_message(current_uid,current_chatting_id,msg_arr);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
