#-------------------------------------------------
#
# Project created by QtCreator 2013-04-19T15:34:30
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientGUI
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    cloudportal.cpp

HEADERS  += \
    mainwindow.h \
    cloudportal.h

FORMS    += mainwindow.ui \
    cloudportal.ui

RESOURCES += \
    resource.qrc
