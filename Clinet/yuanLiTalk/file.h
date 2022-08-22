#ifndef FILE_H
#define FILE_H

#include <QMainWindow>
#include "all.h"

namespace Ui {
class file;
}

class file : public QMainWindow
{
    Q_OBJECT

public:
    explicit file(QWidget *parent = 0);
    ~file();

private slots:
    void on_close_clicked();

    void on_maxsize_clicked();

    void on_minsize_clicked();

    void on_select_file_clicked();

    void hadConnected();

    void on_send_clicked();

private:
    Ui::file *ui;
    int showflag = 0;
    QTcpSocket *clinet;
    QTimer timer;
};

#endif // FILE_H
