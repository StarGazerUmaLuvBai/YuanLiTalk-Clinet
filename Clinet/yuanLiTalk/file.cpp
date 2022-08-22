#include "file.h"
#include "ui_file.h"

file::file(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::file)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    clinet = new QTcpSocket();
    clinet->connectToHost("192.168.1.107",10086);  //173.82.246.214
    connect(clinet,SIGNAL(connected()),this,SLOT(hadConnected()));
}

file::~file()
{
    delete ui;
}

void file::hadConnected()
{
    ui->select_file->setEnabled(true);
}

void file::on_close_clicked()
{
    close();
}

void file::on_maxsize_clicked()
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

void file::on_minsize_clicked()
{
    showMinimized();
}

void file::on_select_file_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,QStringLiteral("文件"),"C:",QStringLiteral("图片文件(*png *jpg);;""文本文件(*txt)"));
    ui->file_name->setText(fileName);
    ui->send->setEnabled(true);
}

void file::on_send_clicked()
{
    QString filepath = ui->file_name->text();
    QFile *f = new QFile();
    f->setFileName(filepath);
    bool isOk = f->open(QIODevice::ReadOnly);
    if(false == isOk)//打开文件失败
    {
        QMessageBox::warning(this,"警告","打开文件失败");
        return;
    }
    //获取发送文件的信息
    QFileInfo info(filepath);
    QString fileName = info.fileName();
    qint64 fileSize = info.size();

    ui->select_file->setEnabled(false);//选择按钮变灰
    ui->send->setEnabled(false);
    QString buff = "{\"operation\": \"fileSend\", "+QString("\"fileName: \"%1\", \"fileSize\": %2}").arg(fileName).arg(fileSize);

    //先发头
    qint64 lenth = clinet->write(buff.toUtf8());
    clinet->waitForBytesWritten();//等待数据发送完毕
    qint64 sendSize = 0;
    if(lenth > 0)
    {
        this->timer.start(1000);
        qint64 len = 0 ;

            do{
                //每次发送2kb大小的数据，如果剩余的数据不足2kb，就发送剩余数据的大小
                char buf[2*1024]= {0};

                len = 0;
                len=f->read(  buf , sizeof(buf) );        //读数据
                len=clinet->write(buf , len);            //发数据

                //发送数据需要累积
                sendSize += len;

            }while(len>0 );
            //文件数据发送完毕
            if (fileSize == sendSize)
                {
                QMessageBox::information(this,"ok","文件发送完毕");

                f->close();//关闭文件
                clinet->disconnectFromHost();
                this->hide();
                //关闭客户端
            }
    }
    else
    {
        f->close();
    }
}
