#ifndef FILESTORAGE_H
#define FILESTORAGE_H
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>

#include <QDebug>
#include <QJsonDocument>
#include <QBuffer>
#include <QPixmap>
#include <QImageReader>
void update_message(int uid,int friend_uid,const QJsonArray &a);
void update_group_message(int uid,int gid,const QJsonArray &a);
QJsonArray get_history_group_message(int uid,int gid);
QJsonArray get_history_message(int uid,int friend_uid);
QByteArray pixmap_to_base64(const QPixmap &pixmap);
QPixmap base64_to_pixmap(const QByteArray &bytes_bs64);
#endif // FILESTORAGE_H
