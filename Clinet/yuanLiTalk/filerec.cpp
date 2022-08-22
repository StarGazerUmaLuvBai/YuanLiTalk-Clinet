#include "filerec.h"
#include "ui_filerec.h"

fileRec::fileRec(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fileRec)
{
    ui->setupUi(this);
    clinet = new QTcpSocket();
    clinet->connectToHost("192.168.1.107",10086);  //173.82.246.214
    connect(clinet,SIGNAL(connected()),this,SLOT(hadConnected()));
}

fileRec::~fileRec()
{
    delete ui;
}

void fileRec::hadConnected()
{
    fileSize = 0;
    recSize = 0;
    ui->receive->setEnabled(true);
}

void fileRec::hadReadyRead()
{
    QByteArray buf = clinet->readAll();
    if(isFile == false)
    {
        isFile = true;
        QJsonParseError parseJsonErr;
        QJsonDocument doucument = QJsonDocument::fromJson(buf,&parseJsonErr);
        QJsonObject jsonObject = doucument.object();
        QString fileName = jsonObject["fileName"].toString();
        fileSize = jsonObject["fileSize"].toInt();
        QString path = "./"+fileName;
        qDebug()<<path;
        f = new QFile(path);
        bool isopen = f->open(QIODevice::WriteOnly);
        if(false == isopen)
        {
            isFile = false;
            QMessageBox::warning(this,"失败","创建文件失败");
            return;
        }
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(fileSize/1024);

        ui->status->append(QString("正在接受文件：\n%1").arg(fileName));
    }
    else
    {
        qDebug()<<"开始写入";
        qDebug()<<buf;
        qint64 len = f->write(buf);
        recSize += len;
        ui->progressBar->setValue(recSize/1024);
    }
    if (recSize == fileSize)
    {
        f->close();
        clinet->disconnectFromHost();
        QMessageBox::information(this,"ok","文件接受完毕");
        this->hide();
    }
}

void fileRec::on_receive_clicked()
{
    connect(clinet,SIGNAL(readyRead()),this,SLOT(hadReadyRead()));
}
