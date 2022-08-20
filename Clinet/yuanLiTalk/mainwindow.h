#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "all.h"
#include "talk.h"
#include "information.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(QTcpSocket *sock, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_minsize_clicked();

    void on_maxsize_clicked();

    void on_close_clicked();

    void on_person1_clicked();

    void hadReadyRead();

    void on_information_clicked();

signals:
    void sendRevMsgSig(QString);
private:
    Ui::MainWindow *ui;
    QTcpSocket * clinet;
    int showflag = 0;
};

#endif // MAINWINDOW_H
