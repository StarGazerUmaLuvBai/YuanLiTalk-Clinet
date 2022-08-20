/********************************************************************************
** Form generated from reading UI file 'talk.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TALK_H
#define UI_TALK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Talk
{
public:
    QPushButton *close;
    QPushButton *maxsize;
    QPushButton *minsize;
    QPushButton *clear;
    QPushButton *send;
    QTextEdit *text_receive;
    QLineEdit *text_send;

    void setupUi(QDialog *Talk)
    {
        if (Talk->objectName().isEmpty())
            Talk->setObjectName(QStringLiteral("Talk"));
        Talk->resize(703, 636);
        close = new QPushButton(Talk);
        close->setObjectName(QStringLiteral("close"));
        close->setGeometry(QRect(660, 10, 41, 28));
        maxsize = new QPushButton(Talk);
        maxsize->setObjectName(QStringLiteral("maxsize"));
        maxsize->setGeometry(QRect(610, 10, 41, 28));
        minsize = new QPushButton(Talk);
        minsize->setObjectName(QStringLiteral("minsize"));
        minsize->setGeometry(QRect(560, 10, 41, 28));
        clear = new QPushButton(Talk);
        clear->setObjectName(QStringLiteral("clear"));
        clear->setGeometry(QRect(460, 580, 93, 28));
        send = new QPushButton(Talk);
        send->setObjectName(QStringLiteral("send"));
        send->setGeometry(QRect(570, 580, 93, 28));
        text_receive = new QTextEdit(Talk);
        text_receive->setObjectName(QStringLiteral("text_receive"));
        text_receive->setGeometry(QRect(0, 60, 701, 381));
        text_receive->setReadOnly(true);
        text_send = new QLineEdit(Talk);
        text_send->setObjectName(QStringLiteral("text_send"));
        text_send->setGeometry(QRect(0, 490, 701, 61));
        text_send->setReadOnly(false);

        retranslateUi(Talk);

        QMetaObject::connectSlotsByName(Talk);
    } // setupUi

    void retranslateUi(QDialog *Talk)
    {
        Talk->setWindowTitle(QApplication::translate("Talk", "Dialog", Q_NULLPTR));
        close->setText(QApplication::translate("Talk", "\303\227", Q_NULLPTR));
        maxsize->setText(QApplication::translate("Talk", "\342\226\241", Q_NULLPTR));
        minsize->setText(QApplication::translate("Talk", "-", Q_NULLPTR));
        clear->setText(QApplication::translate("Talk", "\346\270\205\347\251\272", Q_NULLPTR));
        send->setText(QApplication::translate("Talk", "\345\217\221\351\200\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Talk: public Ui_Talk {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TALK_H
