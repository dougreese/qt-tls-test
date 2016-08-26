QT += core
QT += network
QT -= gui

CONFIG += c++11

TARGET = QtTlsTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    sslclient.cpp

HEADERS += \
    sslclient.h
