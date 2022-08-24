#ifndef REGISTER_H
#define REGISTER_H

#include "all.h"
#include "dialog.h"
#include "tcpnetwork.h"
#include "filestorage.h"

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

private:
    Ui::Register *ui;
    QString profilephoto_name;
    QString profilephoto_base64;
};

#endif // REGISTER_H
