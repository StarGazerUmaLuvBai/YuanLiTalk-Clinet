#ifndef TCPNETWORK_H
#define TCPNETWORK_H



#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QLabel>
#include <QtEndian>
#include <QJsonValue>
#include <QJsonObject>
#include <QIcon>
#include <QPixmap>
#include "filestorage.h"
const QString ip="173.82.246.214";
const int port=10086;
class tcpnetwork:public QObject{
    Q_OBJECT
    QTcpSocket *socket;
public:
    int fixed_len_send(const char* s, int size);
    int fixed_len_receive(char* s, int size);
    int sendMessage(const char* s,int size);
    int sendMessage(const QByteArray &s);
    QByteArray receiveMessage();
    void YuanliTalkSend(const QJsonObject &json);
    explicit tcpnetwork(QObject *parent = nullptr);
    ~tcpnetwork();
    QMap<int,QPixmap> profile_map;
signals:
    void receiveDataDone(QByteArray);
    void registerResult(QJsonObject&);
    void loginResult(QJsonObject&);
    void getMessage(QJsonObject&);
    void getGroupMessage(QJsonObject&);
    void searchUserResult(QJsonObject&);
    void searchGroupResult(QJsonObject&);
    void addUserResult(QJsonObject&);
    void addGroupResult(QJsonObject&);
    void addGroupRequest(QJsonObject&);
    void friendRequest(QJsonObject&);
    void getMibaoResult(QJsonObject&);
    void changePasswordResult(QJsonObject&);
    void getAllProfileResult(QJsonObject&);
private:
    int left_read_len;
    QByteArray m_buff;
};

#endif // TCPNETWORK_H
