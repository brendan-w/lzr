#-------------------------------------------------
#
# Project created by QtCreator 2016-02-07T21:25:23
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = frame_editor
TEMPLATE = app

# CONFIG += qt debug

SOURCES += main.cpp\
        frameeditor.cpp\
        frameview.cpp\
        point.cpp\
        colordock.cpp

HEADERS  += frameeditor.h frameview.h colordock.h
