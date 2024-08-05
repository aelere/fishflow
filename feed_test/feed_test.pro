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
    main.cpp \
    ../db/db_access.cpp \
    ../authentication_engine/user.cpp \
    ../authentication_engine/auth_manager.cpp \
    ../post/one_post.cpp \
    ../post/upload.cpp \
    ../feed/user_feed.cpp \

HEADERS += \
    ../db/db_access.h \
    ../authentication_engine/user.h \
    ../authentication_engine/auth_manager.h \
    ../post/one_post.h \
    ../post/upload.h \
    ../feed/user_feed.h \
