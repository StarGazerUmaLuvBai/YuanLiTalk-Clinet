#include "information.h"
#include "ui_information.h"

information::information(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::information)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}
information::information(QTcpSocket *sock, QString uname, QString id, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::information)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    clinet = sock;
    connect(clinet, SIGNAL(readyRead()), this, SLOT(hadReadyRead()));
    username = uname;
    ID = id;
    ui->username->setText(this->username);
    QString phName = "image_test_" + ID + ".jpg";
    QString iconStyleSheet = "border-image: url(:/image/" + phName + ");";
    ui->image->setStyleSheet(iconStyleSheet);
}

void information::hadReadyRead()
{
    QByteArray msgArr = clinet->readAll();
    QJsonParseError parseJsonErr;
    QJsonDocument doucument = QJsonDocument::fromJson(msgArr,&parseJsonErr);
    QJsonObject jsonObject = doucument.object();
    QString operation = jsonObject["operation"].toString();
    int status = jsonObject["status"].toInt();
    if(operation == "photoChange")
    {
        emit toPhChange(status);
    }
    else if(operation == "passwordChange")
    {
        emit toPWordChange(status);
    }
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
    connect(this, SIGNAL(toPWordChange(int)), pw, SLOT(RecStatus(int)));
    pw->show();
}

void information::on_contact_clicked()
{
    MainWindow *mw = new MainWindow(clinet);
    disconnect(clinet, 0, this, 0);
    mw->username = username;
    mw->ID = ID;
    mw->show();
    this->hide();
}

void information::on_pushButton_clicked()
{
    photo *pH = new photo(clinet);
    connect(pH, SIGNAL(idType(QString)), this, SLOT(phChange(QString)));
    connect(this, SIGNAL(toPhChange(int)), pH, SLOT(RecStatus(int)));
    pH->show();
}

void information::phChange(QString id)
{
    //border-image: url(:/image/image_test_0.jpg);
    ID = id;
    QString phName = "image_test_" + id + ".jpg";
    QString iconStyleSheet = "border-image: url(:/image/" + phName + ");";
    ui->image->setStyleSheet(iconStyleSheet);
}
