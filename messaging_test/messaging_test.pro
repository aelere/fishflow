TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
#CONFIG -= qt

QT += core
QT += sql

# if you have pthread errors, comment/uncomment the next line
LIBS += -pthread
SOURCES += ../gtest/googletest/src/gtest-all.cc
INCLUDEPATH += ../gtest \
../gtest/googletest \
../gtest/googletest/include \
../gtest/googletest/include/gtest

SOURCES += \
        ../messaging/conversation.cpp \
        ../db/db_access.cpp \
        ../authentication_engine/user.cpp \
        ../authentication_engine/auth_manager.cpp \
        main.cpp \
        ../messaging/one_conversation.cpp \
        ../messaging/one_message.cpp

HEADERS += \
        ../messaging/conversation.h \
        ../db/db_access.h \
        ../authentication_engine/user.h \
        ../authentication_engine/auth_manager.h \
        main.h \
        ../messaging/one_conversation.h \
        ../messaging/one_message.h
