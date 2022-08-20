#include "information.h"
#include "ui_information.h"

information::information(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::information)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}
information::information(QTcpSocket *sock, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::information)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    clinet = sock;
}

information::~information()
{
    delete ui;
}

void information::on_close_clicked()
{
    close();
}

void information::on_maxsize_clicked()
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

void information::on_minsize_clicked()
{
    showMinimized();
}


void information::on_exit_clicked()
{
    Dialog *dia = new Dialog();
    this->hide();
    dia->show();
}

void information::on_password_change_clicked()
{
    password_change *pw = new password_change(clinet);
    pw->show();
}

void information::on_contact_clicked()
{
    MainWindow *mw = new MainWindow(clinet);
    mw->show();
    this->hide();
}
