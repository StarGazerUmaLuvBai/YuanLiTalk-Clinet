/********************************************************************************
** Form generated from reading UI file 'password_change.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PASSWORD_CHANGE_H
#define UI_PASSWORD_CHANGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_password_change
{
public:
    QLineEdit *pswd_first;
    QLineEdit *pswd_last;
    QLabel *label;
    QLabel *label_2;
    QPushButton *yes;
    QPushButton *no;

    void setupUi(QDialog *password_change)
    {
        if (password_change->objectName().isEmpty())
            password_change->setObjectName(QStringLiteral("password_change"));
        password_change->resize(488, 440);
        pswd_first = new QLineEdit(password_change);
        pswd_first->setObjectName(QStringLiteral("pswd_first"));
        pswd_first->setGeometry(QRect(180, 140, 113, 21));
        pswd_last = new QLineEdit(password_change);
        pswd_last->setObjectName(QStringLiteral("pswd_last"));
        pswd_last->setGeometry(QRect(180, 200, 113, 21));
        label = new QLabel(password_change);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(100, 150, 72, 15));
        label_2 = new QLabel(password_change);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(90, 200, 81, 20));
        yes = new QPushButton(password_change);
        yes->setObjectName(QStringLiteral("yes"));
        yes->setGeometry(QRect(110, 330, 93, 28));
        no = new QPushButton(password_change);
        no->setObjectName(QStringLiteral("no"));
        no->setGeometry(QRect(250, 330, 93, 28));

        retranslateUi(password_change);

        QMetaObject::connectSlotsByName(password_change);
    } // setupUi

    void retranslateUi(QDialog *password_change)
    {
        password_change->setWindowTitle(QApplication::translate("password_change", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("password_change", "\346\226\260\345\257\206\347\240\201", Q_NULLPTR));
        label_2->setText(QApplication::translate("password_change", "\347\241\256\350\256\244\346\226\260\345\257\206\347\240\201", Q_NULLPTR));
        yes->setText(QApplication::translate("password_change", "\347\241\256\350\256\244", Q_NULLPTR));
        no->setText(QApplication::translate("password_change", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class password_change: public Ui_password_change {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASSWORD_CHANGE_H
