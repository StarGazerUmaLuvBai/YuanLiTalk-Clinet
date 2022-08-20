#include "file.h"
#include "ui_file.h"

file::file(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::file)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

file::~file()
{
    delete ui;
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
