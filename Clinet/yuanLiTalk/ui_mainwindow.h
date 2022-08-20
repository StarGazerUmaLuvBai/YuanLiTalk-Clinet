/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *minsize;
    QPushButton *maxsize;
    QPushButton *close;
    QPushButton *file_reserve;
    QPushButton *contact;
    QPushButton *information;
    QPushButton *person1;
    QPushButton *person2;
    QMenuBar *menubar;
    QMenu *menu_Talk;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        minsize = new QPushButton(centralwidget);
        minsize->setObjectName(QStringLiteral("minsize"));
        minsize->setGeometry(QRect(680, 0, 31, 28));
        maxsize = new QPushButton(centralwidget);
        maxsize->setObjectName(QStringLiteral("maxsize"));
        maxsize->setGeometry(QRect(720, 0, 31, 28));
        close = new QPushButton(centralwidget);
        close->setObjectName(QStringLiteral("close"));
        close->setGeometry(QRect(760, 0, 31, 28));
        file_reserve = new QPushButton(centralwidget);
        file_reserve->setObjectName(QStringLiteral("file_reserve"));
        file_reserve->setGeometry(QRect(0, 510, 251, 41));
        contact = new QPushButton(centralwidget);
        contact->setObjectName(QStringLiteral("contact"));
        contact->setGeometry(QRect(250, 510, 291, 41));
        information = new QPushButton(centralwidget);
        information->setObjectName(QStringLiteral("information"));
        information->setGeometry(QRect(540, 510, 261, 41));
        person1 = new QPushButton(centralwidget);
        person1->setObjectName(QStringLiteral("person1"));
        person1->setGeometry(QRect(90, 100, 93, 28));
        person2 = new QPushButton(centralwidget);
        person2->setObjectName(QStringLiteral("person2"));
        person2->setGeometry(QRect(90, 160, 93, 28));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        menu_Talk = new QMenu(menubar);
        menu_Talk->setObjectName(QStringLiteral("menu_Talk"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu_Talk->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        minsize->setText(QApplication::translate("MainWindow", "-", Q_NULLPTR));
        maxsize->setText(QApplication::translate("MainWindow", "\342\226\241", Q_NULLPTR));
        close->setText(QApplication::translate("MainWindow", "\303\227", Q_NULLPTR));
        file_reserve->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\345\202\250\345\255\230", Q_NULLPTR));
        contact->setText(QApplication::translate("MainWindow", "\350\201\224\347\263\273\344\272\272", Q_NULLPTR));
        information->setText(QApplication::translate("MainWindow", "\344\270\252\344\272\272\344\270\255\345\277\203", Q_NULLPTR));
        person1->setText(QApplication::translate("MainWindow", "person1", Q_NULLPTR));
        person2->setText(QApplication::translate("MainWindow", "person2", Q_NULLPTR));
        menu_Talk->setTitle(QApplication::translate("MainWindow", "\347\214\277\347\220\206Talk", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
