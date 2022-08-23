#-------------------------------------------------
#
# Project created by QtCreator 2022-08-19T19:01:42
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yuanLiTalk
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        dialog.cpp \
    register.cpp \
    information.cpp \
    file.cpp \
    talk.cpp \
    password_change.cpp \
    photo.cpp \
    filerec.cpp \
    tcpnetwork.cpp \
    mainwindow.cpp \
    filestorage.cpp

HEADERS  += dialog.h \
    register.h \
    all.h \
    information.h \
    file.h \
    talk.h \
    password_change.h \
    photo.h \
    filerec.h \
    tcpnetwork.h \
    mainwindow.h \
    filestorage.h

FORMS    += dialog.ui \
    register.ui \
    information.ui \
    file.ui \
    talk.ui \
    password_change.ui \
    photo.ui \
    filerec.ui \
    mainwindow.ui

RESOURCES += \
    image.qrc


