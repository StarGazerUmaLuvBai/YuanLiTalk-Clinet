#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "all.h"
#include "talk.h"
#include "information.h"
#include "file.h"
#include "filerec.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(QTcpSocket *sock, QString u, QWidget *parent = 0);
    ~MainWindow();

    QString ID = "0";
    QString uid;

private slots:
    void on_minsize_clicked();

    void on_maxsize_clicked();

    void on_close_clicked();

    void hadReadyRead();

    void on_information_clicked();

    void on_file_reserve_clicked();

    void on_file_rec_clicked();

    void idChange(QString);
signals:
    void sendRevMsgSig(QString);

    void photoChange(int);

    void passwordChange(int);
private:
    Ui::MainWindow *ui;
    QTcpSocket * clinet;
    int showflag = 0;
};

#endif // MAINWINDOW_H
