TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
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
        main.cpp \
        ../authentication_engine/user.cpp \
        ../authentication_engine/auth_manager.cpp \
        ../moderation/moderation_manager.cpp \
        ../moderation/moderator.cpp \
        ../db/db_access.cpp \

HEADERS += \
        ../authentication_engine/user.h \
        ../authentication_engine/auth_manager.h \
        ../moderation/moderation_manager.h \
        ../moderation/moderator.h \
        ../db/db_access.h \

