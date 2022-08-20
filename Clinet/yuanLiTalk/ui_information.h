/********************************************************************************
** Form generated from reading UI file 'information.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFORMATION_H
#define UI_INFORMATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_information
{
public:
    QWidget *centralwidget;
    QPushButton *file_reserve;
    QPushButton *close;
    QPushButton *maxsize;
    QPushButton *information_2;
    QPushButton *contact;
    QPushButton *minsize;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;
    QPushButton *password_change;
    QPushButton *exit;
    QLineEdit *lineEdit_3;
    QLabel *label_3;
    QLineEdit *lineEdit_4;
    QCalendarWidget *calendarWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *information)
    {
        if (information->objectName().isEmpty())
            information->setObjectName(QStringLiteral("information"));
        information->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(information->sizePolicy().hasHeightForWidth());
        information->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(information);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        file_reserve = new QPushButton(centralwidget);
        file_reserve->setObjectName(QStringLiteral("file_reserve"));
        file_reserve->setGeometry(QRect(0, 510, 251, 41));
        close = new QPushButton(centralwidget);
        close->setObjectName(QStringLiteral("close"));
        close->setGeometry(QRect(760, 0, 31, 28));
        maxsize = new QPushButton(centralwidget);
        maxsize->setObjectName(QStringLiteral("maxsize"));
        maxsize->setGeometry(QRect(720, 0, 31, 28));
        information_2 = new QPushButton(centralwidget);
        information_2->setObjectName(QStringLiteral("information_2"));
        information_2->setGeometry(QRect(540, 510, 261, 41));
        contact = new QPushButton(centralwidget);
        contact->setObjectName(QStringLiteral("contact"));
        contact->setGeometry(QRect(250, 510, 291, 41));
        minsize = new QPushButton(centralwidget);
        minsize->setObjectName(QStringLiteral("minsize"));
        minsize->setGeometry(QRect(680, 0, 31, 28));
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setEnabled(true);
        lineEdit->setGeometry(QRect(80, 50, 113, 21));
        lineEdit->setReadOnly(true);
        lineEdit_2 = new QLineEdit(centralwidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(80, 100, 113, 21));
        lineEdit_2->setReadOnly(true);
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 50, 31, 16));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 100, 31, 16));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(60, 190, 93, 28));
        password_change = new QPushButton(centralwidget);
        password_change->setObjectName(QStringLiteral("password_change"));
        password_change->setGeometry(QRect(60, 260, 93, 28));
        exit = new QPushButton(centralwidget);
        exit->setObjectName(QStringLiteral("exit"));
        exit->setGeometry(QRect(60, 330, 93, 28));
        lineEdit_3 = new QLineEdit(centralwidget);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(490, 60, 113, 21));
        lineEdit_3->setReadOnly(true);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(410, 60, 72, 15));
        lineEdit_4 = new QLineEdit(centralwidget);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(260, 60, 113, 21));
        lineEdit_4->setReadOnly(true);
        calendarWidget = new QCalendarWidget(centralwidget);
        calendarWidget->setObjectName(QStringLiteral("calendarWidget"));
        calendarWidget->setGeometry(QRect(250, 120, 471, 331));
        information->setCentralWidget(centralwidget);
        file_reserve->raise();
        maxsize->raise();
        contact->raise();
        minsize->raise();
        close->raise();
        information_2->raise();
        lineEdit->raise();
        lineEdit_2->raise();
        label->raise();
        label_2->raise();
        pushButton->raise();
        password_change->raise();
        exit->raise();
        lineEdit_3->raise();
        label_3->raise();
        lineEdit_4->raise();
        calendarWidget->raise();
        menubar = new QMenuBar(information);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        information->setMenuBar(menubar);
        statusbar = new QStatusBar(information);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        information->setStatusBar(statusbar);

        retranslateUi(information);

        QMetaObject::connectSlotsByName(information);
    } // setupUi

    void retranslateUi(QMainWindow *information)
    {
        information->setWindowTitle(QApplication::translate("information", "MainWindow", Q_NULLPTR));
        file_reserve->setText(QApplication::translate("information", "\346\226\207\344\273\266\345\202\250\345\255\230", Q_NULLPTR));
        close->setText(QApplication::translate("information", "\303\227", Q_NULLPTR));
        maxsize->setText(QApplication::translate("information", "\342\226\241", Q_NULLPTR));
        information_2->setText(QApplication::translate("information", "\344\270\252\344\272\272\344\270\255\345\277\203", Q_NULLPTR));
        contact->setText(QApplication::translate("information", "\350\201\224\347\263\273\344\272\272", Q_NULLPTR));
        minsize->setText(QApplication::translate("information", "-", Q_NULLPTR));
        label->setText(QApplication::translate("information", "\345\247\223\345\220\215", Q_NULLPTR));
        label_2->setText(QApplication::translate("information", "\346\200\247\345\210\253", Q_NULLPTR));
        pushButton->setText(QApplication::translate("information", "\344\272\213\344\273\266\350\256\276\347\275\256", Q_NULLPTR));
        password_change->setText(QApplication::translate("information", "\346\233\264\346\224\271\345\257\206\347\240\201", Q_NULLPTR));
        exit->setText(QApplication::translate("information", "\351\200\200\345\207\272", Q_NULLPTR));
        label_3->setText(QApplication::translate("information", "username", Q_NULLPTR));
        lineEdit_4->setText(QApplication::translate("information", "\345\244\264\345\203\217", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class information: public Ui_information {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFORMATION_H
