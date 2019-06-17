#-------------------------------------------------
#
# Project created by QtCreator 2019-06-16T21:07:30
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = KNN_cplusplus
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    CSV.cpp \
    CSVManager.cpp

HEADERS += \
    CSV.h \
    CSVManager.h
QMAKE_CXXFLAGS += -std=c++11
