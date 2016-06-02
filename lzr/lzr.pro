QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lzr
TEMPLATE = app

CONFIG += qt debug
MOC_DIR = build/
OBJECTS_DIR = build/

SOURCES += src/*.cpp
HEADERS += src/*.h
