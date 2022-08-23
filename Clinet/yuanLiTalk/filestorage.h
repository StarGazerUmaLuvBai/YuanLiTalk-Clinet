#ifndef FILESTORAGE_H
#define FILESTORAGE_H
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>
void update_message(int uid,int friend_uid,const QJsonArray &a);
void update_group_message(int uid,int gid,const QJsonArray &a);

#endif // FILESTORAGE_H
