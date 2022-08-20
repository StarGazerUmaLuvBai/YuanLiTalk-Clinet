/********************************************************************************
** Form generated from reading UI file 'file.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_H
#define UI_FILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_file
{
public:
    QWidget *centralwidget;
    QPushButton *file_reserve;
    QPushButton *contact;
    QPushButton *information;
    QPushButton *minsize;
    QPushButton *maxsize;
    QPushButton *close;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *file)
    {
        if (file->objectName().isEmpty())
            file->setObjectName(QStringLiteral("file"));
        file->resize(800, 600);
        centralwidget = new QWidget(file);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        file_reserve = new QPushButton(centralwidget);
        file_reserve->setObjectName(QStringLiteral("file_reserve"));
        file_reserve->setGeometry(QRect(0, 510, 251, 41));
        contact = new QPushButton(centralwidget);
        contact->setObjectName(QStringLiteral("contact"));
        contact->setGeometry(QRect(250, 510, 291, 41));
        information = new QPushButton(centralwidget);
        information->setObjectName(QStringLiteral("information"));
        information->setGeometry(QRect(540, 510, 261, 41));
        minsize = new QPushButton(centralwidget);
        minsize->setObjectName(QStringLiteral("minsize"));
        minsize->setGeometry(QRect(680, 0, 31, 28));
        maxsize = new QPushButton(centralwidget);
        maxsize->setObjectName(QStringLiteral("maxsize"));
        maxsize->setGeometry(QRect(720, 0, 31, 28));
        close = new QPushButton(centralwidget);
        close->setObjectName(QStringLiteral("close"));
        close->setGeometry(QRect(760, 0, 31, 28));
        file->setCentralWidget(centralwidget);
        menubar = new QMenuBar(file);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        file->setMenuBar(menubar);
        statusbar = new QStatusBar(file);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        file->setStatusBar(statusbar);

        retranslateUi(file);

        QMetaObject::connectSlotsByName(file);
    } // setupUi

    void retranslateUi(QMainWindow *file)
    {
        file->setWindowTitle(QApplication::translate("file", "MainWindow", Q_NULLPTR));
        file_reserve->setText(QApplication::translate("file", "\346\226\207\344\273\266\345\202\250\345\255\230", Q_NULLPTR));
        contact->setText(QApplication::translate("file", "\350\201\224\347\263\273\344\272\272", Q_NULLPTR));
        information->setText(QApplication::translate("file", "\344\270\252\344\272\272\344\270\255\345\277\203", Q_NULLPTR));
        minsize->setText(QApplication::translate("file", "-", Q_NULLPTR));
        maxsize->setText(QApplication::translate("file", "\342\226\241", Q_NULLPTR));
        close->setText(QApplication::translate("file", "\303\227", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class file: public Ui_file {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_H
