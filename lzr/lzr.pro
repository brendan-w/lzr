QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lzr
TEMPLATE = app

CONFIG += qt debug
MOC_DIR = build/
OBJECTS_DIR = build/

SOURCES += src/*.cpp
SOURCES += src/FrameEditor/*.cpp
SOURCES += src/ClipEditor/*.cpp

HEADERS += src/*.hpp
HEADERS += src/FrameEditor/*.hpp
HEADERS += src/ClipEditor/*.hpp

INCLUDEPATH += ../liblzr

# TODO: switch this to the installed path
LIBS += ../liblzr/liblzr.so
# LIBS += -llzr
