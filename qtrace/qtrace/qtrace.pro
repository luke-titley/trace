#-------------------------------------------------
#
# Project created by QtCreator 2013-01-25T08:17:47
#
#-------------------------------------------------

QT       += core gui

TARGET = qtrace
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

unix:!macx:!symbian: LIBS += -L$$PWD/../../trace/lib/  -lthread -ltrace -ltraceshaders -llsd -ltracetest

INCLUDEPATH += $$PWD/../../trace/include
INCLUDEPATH += $$PWD/../../trace/lsd/include
DEPENDPATH += $$PWD/../../trace/lsd/include
DEPENDPATH += $$PWD/../../trace/include
