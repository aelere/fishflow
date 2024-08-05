TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
QT += core
QT += sql

SOURCES += \
        db_access.cpp \
        main.cpp

HEADERS += \
    db_access.h
