#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}
MainWindow::MainWindow(QTcpSocket *sock, QString u, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    uid = u;
    clinet = sock;
    QString packData = "{\"operation\" : \"getInformation\" , \"uid\" : \"" + uid + "\"}";
    clinet->write(packData.toUtf8());
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
    QString operation = jsonObject["operation"].toString();
    if(operation == "information")     //接收协议：{"operation" : "information" , "friendNumber" : 3 , "friendName" : ["user1","user2","user3"] , "friendID" : ["1","2","3"]}
    {
        int fNum = jsonObject["friendNumber"].toInt();
        QJsonValue NAME = jsonObject.value("friendName");
        QJsonValue i_D = jsonObject.value("friendID");  //id是本地头像的编号
        QJsonArray nameAry = NAME.toArray();
        QJsonArray idAry = i_D.toArray();
        QList<QString>nameList;

        QStringList iconNameList;
        int i = 0;
        for(i = 0;i < fNum; i++)
        {
            QJsonValue nitem = nameAry.at(i);
            QJsonValue iditem = idAry.at(i);
            nameList<<nitem.toString();
            iconNameList<<iditem.toString();
        }
        QVector<QToolButton*> vToolBtn;
        for(i=0;i<fNum;i++)
        {
            //设置头像
            QToolButton *btn =new QToolButton;
            btn->setText(nameList[i]);//文字
            //头像:/image/"
            QString str=QString(":/image/image_test_%1.jpg").arg(iconNameList.at(i));
            btn->setIcon(QPixmap(str));
            //头像大小
            btn->setIconSize(QPixmap(str).size());
            //设置按钮风格 透明
            btn->setAutoRaise(true);
            btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            //加到 垂直布局
            ui->verticalLayout->addWidget(btn);
            //容器保存住九个按钮
            vToolBtn.push_back(btn);
        }
        for(int i=0;i<vToolBtn.size();i++)
        {
            connect(vToolBtn[i], &QToolButton::clicked,[=]()mutable
            {

                //弹出聊天对话框
                //构造聊天窗口时告诉名字
                Talk * widget1=new Talk(clinet,vToolBtn[i]->text(),uid);
                widget1->setWindowTitle(vToolBtn[i]->text());
                widget1->setWindowIcon(vToolBtn[i]->icon());
                connect(this, SIGNAL(sendRevMsgSig(QString)), widget1, SLOT(hadReadyRec(QString)));
                widget1->show();
            });

        }
    }
    else if(operation == "recMessage")
    {
        QString msg = jsonObject["message"].toString();
        emit sendRevMsgSig(msg);
    }
    else if(operation == "photoChange")
    {
        int status = jsonObject["status"].toInt();
        emit photoChange(status);
    }
    else if(operation == "passwordChange")
    {
        int status = jsonObject["status"].toInt();
        emit passwordChange(status);
    }
}

void MainWindow::idChange(QString id)
{
    ID = id;
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

void MainWindow::on_information_clicked()
{
    information *inForm = new information(clinet, ID, uid);
    connect(inForm, SIGNAL(IDchange(QString)), this, SLOT(idChange(QString)));
    connect(this, SIGNAL(passwordChange(int)), inForm, SLOT(msgToPwordchange(int)));
    connect(this, SIGNAL(photoChange(int)), inForm, SLOT(msgToPhchange(int)));
    inForm->show();
}

void MainWindow::on_file_reserve_clicked()
{
    file *fDia = new file();
    fDia->show();
}

void MainWindow::on_file_rec_clicked()
{
    fileRec *fRDia = new fileRec;
    fRDia->show();
}
