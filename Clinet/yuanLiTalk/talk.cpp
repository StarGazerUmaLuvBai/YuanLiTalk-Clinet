#include "talk.h"
#include "ui_talk.h"

Talk::Talk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Talk)
{
    ui->setupUi(this);
}
Talk::Talk(QTcpSocket *sock, QString name, QString u, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Talk)
{
    ui->setupUi(this);
    clinet = sock;
    tName = name;
    ui->talkName->setText(name);
    uid = u;
}

Talk::~Talk()
{
    delete ui;
}

void Talk::on_send_clicked()
{
    QString msg = ui->text_send->text();
    QString uname = ui->talkName->text();
    QString packData = "{\"operation\" : \"sendMessage\" , \"sender\" : \""+uid+"\" , \"receiver\" : \""+uname+"\" , \"message\" : \"" + msg + "\"}";
    clinet->write(packData.toUtf8());
    ui->text_receive->insertPlainText("你："+msg+"\n");
}

void Talk::hadReadyRec(QString msg)
{
    QString uname = ui->talkName->text();
    ui->text_receive->insertPlainText(uname+":"+msg+"\n");
}

void Talk::on_clear_clicked()
{
    ui->text_send->clear();
}
