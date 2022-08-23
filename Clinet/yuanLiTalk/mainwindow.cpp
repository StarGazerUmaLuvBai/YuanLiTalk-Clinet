#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
   /* QPushButton *p[10];
    ui->scrollAreaWidgetContents->layout()->setAlignment(Qt::AlignTop);
    for(int i=0;i<10;++i){
        p[i]=new QPushButton;
        p[i]->setText(QString::number(i));
        p[i]->setMinimumSize(100,100);
        ui->scrollAreaWidgetContents->layout()->addWidget(p[i]);
    }
    delete p[3];*/
}

MainWindow::~MainWindow()
{
    delete ui;
}
