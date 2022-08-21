#ifndef PASSWORD_CHANGE_H
#define PASSWORD_CHANGE_H

#include <QDialog>
#include "all.h"

namespace Ui {
class password_change;
}

class password_change : public QDialog
{
    Q_OBJECT

public:
    explicit password_change(QWidget *parent = 0);
    explicit password_change(QTcpSocket *sock, QWidget *parent = 0);
    ~password_change();

private slots:
    void on_no_clicked();

    void on_yes_clicked();

    void RecStatus(int);
private:
    Ui::password_change *ui;
    QTcpSocket *clinet;
};

#endif // PASSWORD_CHANGE_H
