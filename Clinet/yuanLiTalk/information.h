#ifndef INFORMATION_H
#define INFORMATION_H

#include <QMainWindow>
#include "all.h"
#include "dialog.h"
#include "password_change.h"
#include "mainwindow.h"

namespace Ui {
class information;
}

class information : public QMainWindow
{
    Q_OBJECT

public:
    explicit information(QWidget *parent = 0);
    explicit information(QTcpSocket *sock, QWidget *parent = 0);
    ~information();

private slots:
    void on_close_clicked();

    void on_maxsize_clicked();

    void on_minsize_clicked();

    void on_exit_clicked();

    void on_password_change_clicked();

    void on_contact_clicked();

private:
    Ui::information *ui;
    int showflag = 0;
    QTcpSocket *clinet;
};

#endif // INFORMATION_H
