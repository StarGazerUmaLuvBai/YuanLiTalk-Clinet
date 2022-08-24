#ifndef DIALOG_H
#define DIALOG_H

#include "all.h"
#include "register.h"
#include "mainwindow.h"

#include "tcpnetwork.h"
extern tcpnetwork *socket;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    int uid;
    QString passWord;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Dialog *ui;
    bool is_token_mod;
};

#endif // DIALOG_H

