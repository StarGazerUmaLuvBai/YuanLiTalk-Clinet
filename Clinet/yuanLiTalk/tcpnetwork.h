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

const QString ip="192.168.81.129";
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
signals:
    void registerResult(QJsonObject);
    void loginResult(QJsonObject);
};

#endif // TCPNETWORK_H
