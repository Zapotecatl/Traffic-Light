#-------------------------------------------------
#
# Project created by QtCreator 2011-10-13T04:41:13
#
#-------------------------------------------------

QT += gui declarative

QT       += core gui

TARGET = QtTrafficLights
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    traffic_light.cpp \
    vehicle.cpp \
    sensor.cpp \
    measures.cpp

HEADERS  += mainwindow.h \
    traffic_light.h \
    vehicle.h \
    sensor.h \
    measures.h

FORMS    += mainwindow.ui

RESOURCES += \
    imagenes.qrc
