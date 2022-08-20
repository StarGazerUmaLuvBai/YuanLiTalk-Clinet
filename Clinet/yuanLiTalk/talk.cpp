#include "talk.h"
#include "ui_talk.h"

Talk::Talk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Talk)
{
    ui->setupUi(this);
}
Talk::Talk(QTcpSocket *sock, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Talk)
{
    ui->setupUi(this);
    clinet = sock;
}

Talk::~Talk()
{
    delete ui;
}

void Talk::on_send_clicked()
{
    QString msg = ui->text_send->text();
    QString packData = "{\"operation\" : \"sendMessage\" , \"message\" : \"" + msg + "\"}";
    clinet->write(packData.toUtf8());

}

void Talk::hadReadyRec(QString msg)
{
    ui->text_receive->insertPlainText(msg+"\n");
}

void Talk::on_clear_clicked()
{
    ui->text_send->clear();
}
