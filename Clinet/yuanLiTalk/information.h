#ifndef INFORMATION_H
#define INFORMATION_H

#include <QMainWindow>
#include "all.h"
#include "dialog.h"
#include "password_change.h"
#include "mainwindow.h"
#include "photo.h"

namespace Ui {
class information;
}

class information : public QMainWindow
{
    Q_OBJECT

public:
    explicit information(QWidget *parent = 0);
    explicit information(QTcpSocket *sock, QString uname, QString id, QWidget *parent = 0);
    ~information();

    QString username;
    QString ID = "0";

private slots:
    void on_close_clicked();

    void on_maxsize_clicked();

    void on_minsize_clicked();

    void on_exit_clicked();

    void on_password_change_clicked();

    void on_contact_clicked();

    void on_pushButton_clicked();

    void phChange(QString);

    void hadReadyRead();
signals:
    void toPhChange(int);

    void toPWordChange(int);
private:
    Ui::information *ui;
    int showflag = 0;
    QTcpSocket *clinet;
};

#endif // INFORMATION_H
