#include "information.h"
#include "ui_information.h"

information::information(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::information)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}
information::information(QTcpSocket *sock, QString id,QString u, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::information)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    clinet = sock;
    ID = id;
    uid = u;
    ui->username->setText(uid);
    QString phName = "image_test_" + ID + ".jpg";
    QString iconStyleSheet = "border-image: url(:/image/" + phName + ");";
    ui->image->setStyleSheet(iconStyleSheet);
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
    password_change *pw = new password_change();
    connect(this, SIGNAL(toPWordChange(int)), pw, SLOT(RecStatus(int)));
    pw->show();
}

void information::on_contact_clicked()
{
    this->hide();
}

void information::on_pushButton_clicked()
{
    photo *pH = new photo(clinet);
    connect(pH, SIGNAL(idType(QString)), this, SLOT(phChange(QString)));
    connect(this, SIGNAL(toPhChange(int)), pH, SLOT(RecStatus(int)));
    pH->show();
}

void information::msgToPhchange(int status)
{
    emit toPhChange(status);
}

void information::msgToPwordchange(int status)
{
    emit toPWordChange(status);
}

void information::phChange(QString id)
{
    //border-image: url(:/image/image_test_0.jpg);
    ID = id;
    emit IDchange(ID);
    QString phName = "image_test_" + id + ".jpg";
    QString iconStyleSheet = "border-image: url(:/image/" + phName + ");";
    ui->image->setStyleSheet(iconStyleSheet);
}
