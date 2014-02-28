#This file is part of Quickpoll.

#Author: Mauro Alessandro (www.maetech.it) (c) march 2014

#Quickpoll is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#QuickPoll is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with QuickPoll.  If not, see <http://www.gnu.org/licenses/>.


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
    archivedelegate.cpp \
    dialog.cpp \
    results.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    archivedelegate.h \
    dialog.h \
    results.h \
    about.h

FORMS    += mainwindow.ui \
    dialog.ui \
    results.ui \
    about.ui

RESOURCES += \
    quickpoll.qrc

OTHER_FILES += \
    quickpoll.conf \
    about.txt
