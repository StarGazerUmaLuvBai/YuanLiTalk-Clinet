#include "photo.h"
#include "ui_photo.h"

photo::photo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::photo)
{
    ui->setupUi(this);
}

photo::photo(QTcpSocket *sock, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::photo)
{
    ui->setupUi(this);
    clinet = sock;
}

photo::~photo()
{
    delete ui;
}

void photo::on_pushButton_clicked()
{
    id = "1";
    QString packData = "{\"operation\" : \"photoChange\" , \"photoID\" : \"" + id + "\"}";
    clinet->write(packData.toUtf8());
}

void photo::on_pushButton_2_clicked()
{
    id = "2";
    QString packData = "{\"operation\" : \"photoChange\" , \"photoID\" : \"" + id + "\"}";
    clinet->write(packData.toUtf8());
}
void photo::on_pushButton_3_clicked()
{
    id = "3";
    QString packData = "{\"operation\" : \"photoChange\" , \"photoID\" : \"" + id + "\"}";
    clinet->write(packData.toUtf8());
}

void photo::RecStatus(int status)
{
    if(status == 1)
    {
        QMessageBox::warning(this,"错误","修改失败，请稍后再试");
    }
    else
    {
        emit idType(id);
        QMessageBox::information(this,"成功","修改成功");
        this->hide();
    }
}
