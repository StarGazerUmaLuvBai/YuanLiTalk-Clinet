#ifndef PHOTO_H
#define PHOTO_H

#include <QDialog>
#include "all.h"

namespace Ui {
class photo;
}

class photo : public QDialog
{
    Q_OBJECT

public:
    explicit photo(QWidget *parent = 0);
    explicit photo(QTcpSocket *sock, QWidget *parent = 0);
    ~photo();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void RecStatus(int);
signals:
    void idType(QString);
private:
    Ui::photo *ui;
    QString id;
    QTcpSocket *clinet;
};

#endif // PHOTO_H
