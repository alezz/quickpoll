#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T19:12:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = quickpoll
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    archivedelegate.cpp

HEADERS  += mainwindow.h \
    archivedelegate.h

FORMS    += mainwindow.ui

RESOURCES += \
    quickpoll.qrc

OTHER_FILES += \
    quickpoll.conf
