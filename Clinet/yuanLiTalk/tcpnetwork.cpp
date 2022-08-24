#include "tcpnetwork.h"

tcpnetwork::tcpnetwork(QObject *parent) : QObject(parent){
    socket=new QTcpSocket(this);
    socket->connectToHost(ip,port);
    connect(socket,QTcpSocket::readyRead,[=](){
        QByteArray a=receiveMessage();
        QJsonDocument doc=QJsonDocument::fromJson(a);
        QJsonObject response=doc.object();
        if(response.find("signal")!=response.end()){
            QString resp_signal=response["signal"].toString();
            if(resp_signal=="registerResult"){
                emit registerResult(response);
            }
            else if(resp_signal=="loginResult"){
                emit loginResult(response);
            }
            else if(resp_signal=="getMessage"){
                emit getMessage(response);
            }
            else if(resp_signal=="getGroupMessage"){
                emit getGroupMessage(response);
            }
            else if(resp_signal=="searchUserResult"){
                emit searchUserResult(response);
            }
            else if(resp_signal=="searchGroupResult"){
                emit searchGroupResult(response);
            }
        }
        qDebug()<<a.data();
    });
}

tcpnetwork::~tcpnetwork(){

}
int tcpnetwork::fixed_len_receive(char* s, int size){
    int left =size;
    char *p=s;
    while (left > 0) {
        qDebug()<<left;
        int len = socket->read(p,left);
        if (len < 0) {
            return -1;
        }
        else if (len == 0) {
            continue;
        }
        else {
            p += len;
            left -= len;
        }
    }
    return size;
}

int tcpnetwork::fixed_len_send(const char* s, int size){
    int left = size;
    const char *p=s;
    while(left>0){
        int len = socket->write(p,left);
        if(len<0){
            return -1;
        }
        else if(len==0){
            continue;
        }
        else{
            p+=len;
            left-=len;
        }
    }
    return size;
}
int tcpnetwork::sendMessage(const char* s,int size){

    qint32 size_1=qToBigEndian(size);
    char *buff=new char[size+4];
    memcpy(buff,&size_1,4);
    memcpy(buff+4,s,size);
    if(fixed_len_send(buff,size+4)!=size+4){
        qDebug()<< "发送失败";
    }
    delete buff;
    return size;
}
int tcpnetwork::sendMessage(const QByteArray &s){
    qDebug()<<"tcpnetwork::sendMessage(const QByteArray &s)";
    sendMessage(s.data(),s.size());
    return s.size();
}
QByteArray tcpnetwork::receiveMessage(){
    qint32 size;

    fixed_len_receive((char *)&size,4);

    size = qFromBigEndian(size);
    char *s=new char[size];
    qDebug()<<size;
    if(fixed_len_receive(s,size)!=size){
        qDebug()<< "接收失败";
    }
    QByteArray res(s,size);
    delete s;

    qDebug()<<size;
    return res;
}
void tcpnetwork::YuanliTalkSend(const QJsonObject &json){
    QJsonDocument doc(json);
    QByteArray s = doc.toJson();
    qDebug()<<s.data()<<'\n'<<s.size();
    sendMessage(s);


    // return QJsonDocument::fromJson("{'status':0}").object();
    // return QJsonDocument::fromJson(receiveMessage()).object();
}
