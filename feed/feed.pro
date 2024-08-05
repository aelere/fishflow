TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
#CONFIG -= qt

QT += core
QT += sql

SOURCES += \
        main.cpp \
        user_feed.cpp \
        ../db/db_access.cpp \
        ../authentication_engine/user.cpp \
        ../authentication_engine/auth_manager.cpp \
        ../post/one_post.cpp \

HEADERS += \
    user_feed.h \
    user_feed.h \
    ../db/db_access.h \
    ../authentication_engine/user.h \
    ../authentication_engine/auth_manager.h \
    ../post/one_post.h \

