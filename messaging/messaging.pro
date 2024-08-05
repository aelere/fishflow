TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
#CONFIG -= qt

QT += core
QT += sql

SOURCES += \
        conversation.cpp \
        ../db/db_access.cpp \
        ../authentication_engine/user.cpp \
        ../authentication_engine/auth_manager.cpp \
        main.cpp \
        one_conversation.cpp \
        one_message.cpp

HEADERS += \
    conversation.h \
    ../db/db_access.h \
    ../authentication_engine/user.h \
    ../authentication_engine/auth_manager.h \
    one_conversation.h \
    one_message.h
