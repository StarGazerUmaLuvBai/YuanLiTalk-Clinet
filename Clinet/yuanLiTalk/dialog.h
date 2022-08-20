#ifndef DIALOG_H
#define DIALOG_H

#include "all.h"
#include "register.h"
#include "mainwindow.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void hadConnected();
    void hadReadyRead();

private:
    Ui::Dialog *ui;
    QTcpSocket *clinet;
};

#endif // DIALOG_H

