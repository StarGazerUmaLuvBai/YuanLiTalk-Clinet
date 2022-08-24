#include "tcpnetwork.h"

tcpnetwork::tcpnetwork(QObject *parent) : QObject(parent){
    socket=new QTcpSocket(this);
    socket->connectToHost(ip,port);
    socket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 1024*1024*100);
    qDebug()<<"b"<<socket->readBufferSize();

    left_read_len=0;
    connect(socket,QTcpSocket::readyRead,[=](){
        m_buff.append(socket->readAll());
        if(m_buff.size()<4) return;
        quint32 len = qFromBigEndian(*(qint32*)m_buff.data());
        if (len > 1024 * 1024 * 1024) throw "错误数据";
        if (m_buff.size() >= len+4)
        {
            emit receiveDataDone(m_buff.mid(4,len));
            m_buff.remove(0,len+4);
        }

    });
    connect(this,tcpnetwork::receiveDataDone,[=](QByteArray a){

        // QByteArray a=receiveMessage();
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
            else if(resp_signal=="addUserResult"){
                emit addUserResult(response);
            }
            else if(resp_signal=="addGroupResult"){
                emit addGroupResult(response);
            }
            else if(resp_signal=="friendRequest"){
                emit friendRequest(response);
            }
            else if(resp_signal=="getMibaoResult"){
                emit getMibaoResult(response);
            }
            else if(resp_signal=="changePasswordResult"){
                emit changePasswordResult(response);
            }
            else if(resp_signal=="getAllProfileResult"){
                emit getAllProfileResult(response);
            }
        }
        qDebug()<<a.data();
    });

    connect(this,getAllProfileResult,[=](const QJsonObject&resp){
        QJsonObject profiles=resp["profiles"].toObject();
        QStringList keys=profiles.keys();
        for(QString key:keys){
            if(profiles[key]==""){
                profile_map[key.toInt()]=QPixmap(":/img/default_profilephoto.jpg","jpg");
            }
            else{
                profile_map[key.toInt()]= base64_to_pixmap(profiles[key].toString().toUtf8());
            }

        }

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
            //throw "接受失败"; //continue;
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
}
