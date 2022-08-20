#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}
MainWindow::MainWindow(QTcpSocket *sock, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    clinet = sock;
    connect(clinet, SIGNAL(readyRead()), this, SLOT(hadReadyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hadReadyRead()
{
    QByteArray msgArr = clinet->readAll();
    QJsonParseError parseJsonErr;
    QJsonDocument doucument = QJsonDocument::fromJson(msgArr,&parseJsonErr);
    QJsonObject jsonObject = doucument.object();
    QString msg = jsonObject["message"].toString();
    emit sendRevMsgSig(msg);
}

void MainWindow::on_minsize_clicked()
{
    showMinimized();
}

void MainWindow::on_maxsize_clicked()
{
    if(showflag == 0)
    {
        showFullScreen();
        showflag = 1;
    }
    else
    {
        showNormal();
        showflag = 0;
    }
}

void MainWindow::on_close_clicked()
{
    close();
}

void MainWindow::on_person1_clicked()
{
    Talk *tk1 = new Talk(clinet);
    connect(this, SIGNAL(sendRevMsgSig(QString)), tk1, SLOT(hadReadyRec(QString)));
    tk1->show();

}

void MainWindow::on_information_clicked()
{
    information *inForm = new information(clinet);

    inForm->show();
    this->hide();
}
