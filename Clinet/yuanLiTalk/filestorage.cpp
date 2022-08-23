#include "filestorage.h"
#include <QDebug>
#include <QJsonDocument>
QJsonArray get_history_message(int uid,int friend_uid){
    QDir dataDir;
    if(!dataDir.exists("userData")){
        dataDir.mkdir("userData");
    }
    dataDir.cd("userData");
    if(!dataDir.exists(QString::number(uid))){
       dataDir.mkdir(QString::number(uid));
    }
    dataDir.cd(QString::number(uid));

    if(!dataDir.exists("message")){
        dataDir.mkdir("message");
    }
    dataDir.cd("message");

    if(!dataDir.exists(QString::number(friend_uid))){
        QFile file(dataDir.filePath(QString::number(friend_uid)));
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("[]");
        file.close();
    }
    QFile file(dataDir.filePath(QString::number(friend_uid)));

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray arr = file.readAll();
    file.close();

    QJsonArray org= QJsonDocument::fromJson(arr).array();
    return org;
}

void update_message(int uid,int friend_uid,const QJsonArray &a){
    QDir dataDir;
    if(!dataDir.exists("userData")){
        dataDir.mkdir("userData");
    }
    dataDir.cd("userData");
    if(!dataDir.exists(QString::number(uid))){
        dataDir.mkdir(QString::number(uid));
    }
    dataDir.cd(QString::number(uid));

    if(!dataDir.exists("message")){
        dataDir.mkdir("message");
    }
    dataDir.cd("message");

    if(!dataDir.exists(QString::number(friend_uid))){
        QFile file(dataDir.filePath(QString::number(friend_uid)));
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("[]");
        file.close();
    }
    QFile file(dataDir.filePath(QString::number(friend_uid)));

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray arr = file.readAll();
    file.close();
    QJsonArray org= QJsonDocument::fromJson(arr).array();



    for(int i=0;i<a.size();++i){
        org.append(a[i]);
    }
    QFile file_1(dataDir.filePath(QString::number(friend_uid)));
    file_1.open(QIODevice::WriteOnly | QIODevice::Text);
    file_1.write(QJsonDocument(org).toJson(QJsonDocument::Compact));
    file_1.close();
}
QJsonArray get_history_group_message(int uid,int gid){
    QDir dataDir;

    if(!dataDir.exists("userData")){
        dataDir.mkdir("userData");
    }
    dataDir.cd("userData");
    if(!dataDir.exists(QString::number(uid))){
        dataDir.mkdir(QString::number(uid));
    }
    dataDir.cd(QString::number(uid));

    if(!dataDir.exists("groupMessage")){
        dataDir.mkdir("groupMessage");
    }
    dataDir.cd("groupMessage");

    if(!dataDir.exists(QString::number(gid))){
        QFile file(dataDir.filePath(QString::number(gid)));
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("[]");
        file.close();
    }
    QFile file(dataDir.filePath(QString::number(gid)));

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray arr = file.readAll();
    file.close();

    QJsonArray org= QJsonDocument::fromJson(arr).array();
    return org;
}

void update_group_message(int uid,int gid,const QJsonArray &a){
    QDir dataDir;

    if(!dataDir.exists("userData")){
        dataDir.mkdir("userData");
    }
    dataDir.cd("userData");
    if(!dataDir.exists(QString::number(uid))){
        dataDir.mkdir(QString::number(uid));
    }
    dataDir.cd(QString::number(uid));

    if(!dataDir.exists("groupMessage")){
        dataDir.mkdir("groupMessage");
    }
    dataDir.cd("groupMessage");

    if(!dataDir.exists(QString::number(gid))){
        QFile file(dataDir.filePath(QString::number(gid)));
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("[]");
        file.close();
    }
    QFile file(dataDir.filePath(QString::number(gid)));

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray arr = file.readAll();
    file.close();

    QJsonArray org= QJsonDocument::fromJson(arr).array();

    for(int i=0;i<a.size();++i){
        org.append(a[i]);
    }
    QFile file_1(dataDir.filePath(QString::number(gid)));
    file_1.open(QIODevice::WriteOnly | QIODevice::Text);
    file_1.write(QJsonDocument(org).toJson(QJsonDocument::Compact));
    file_1.close();
}
