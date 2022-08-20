#ifndef REGISTER_H
#define REGISTER_H

#include "all.h"
#include "dialog.h"

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void hadConnected();
    void hadReadyRead();

private:
    Ui::Register *ui;
    QTcpSocket *clinet;
};

#endif // REGISTER_H
