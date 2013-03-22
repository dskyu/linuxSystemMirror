#-------------------------------------------------
#
# Project created by QtCreator 2013-02-27T14:10:04
#
#-------------------------------------------------

QT       += core gui

TARGET = Os_4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tool.cpp \
    RefleshThread.cpp \
    curve.cpp

HEADERS  += mainwindow.h \
    tool.h \
    proc.h \
    RefleshThread.h \
    Queue.h \
    curve.h \
    ICurve.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
