#ifndef FILEREC_H
#define FILEREC_H

#include <QDialog>
#include "all.h"

namespace Ui {
class fileRec;
}

class fileRec : public QDialog
{
    Q_OBJECT

public:
    explicit fileRec(QWidget *parent = 0);
    ~fileRec();

private slots:
    void on_receive_clicked();

    void hadConnected();

    void hadReadyRead();

private:
    Ui::fileRec *ui;
    QTcpSocket *clinet;
    bool isFile = false;
    QFile *f;
    qint64 recSize;
    qint64 fileSize;
};

#endif // FILEREC_H
