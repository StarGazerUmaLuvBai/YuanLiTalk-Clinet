#ifndef TALK_H
#define TALK_H

#include <QDialog>
#include "mainwindow.h"
#include "all.h"

namespace Ui {
class Talk;
}

class Talk : public QDialog
{
    Q_OBJECT

public:
    explicit Talk(QWidget *parent = 0);
    explicit Talk(QTcpSocket *sock, QWidget *parent = 0);
    ~Talk();

private slots:
    void on_send_clicked();
    void hadReadyRec(QString);

    void on_clear_clicked();

signals:
    void readySendMsg(QString);
private:
    Ui::Talk *ui;
    QTcpSocket *clinet;
};

#endif // TALK_H
