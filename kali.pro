#-------------------------------------------------
#
# Project created by QtCreator 2015-01-29T10:06:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = kaliset
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    kaliset.cpp \
    imageview.cpp \
    renderthread.cpp \
    kaliview.cpp

HEADERS  += mainwindow.h \
    kaliset.h \
    imageview.h \
    renderthread.h \
    kaliview.h
