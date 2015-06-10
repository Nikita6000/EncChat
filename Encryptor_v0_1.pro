#-------------------------------------------------
#
# Project created by QtCreator 2015-06-01T19:41:20
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Encryptor_v0_1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myclass.cpp \
    tcpclient.cpp \
    dialog.cpp \
    mainmenu.cpp \
    settings.cpp \
    utilityclasses.cpp

HEADERS  += mainwindow.h \
    myclass.h \
    tcpclient.h \
    include.h \
    dialog.h \
    utilityclasses.h \
    mainmenu.h \
    settings.h

FORMS    += mainwindow.ui \
    dialog.ui \
    mainmenu.ui \
    settings.ui
