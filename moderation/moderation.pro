TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
QT += core
QT += sql

SOURCES += \
        main.cpp \
        ../db/db_access.cpp \
        ../authentication_engine/user.cpp \
        ../authentication_engine/auth_manager.cpp \
        ../post/one_post.cpp \
        moderation_manager.cpp \
        moderator.cpp


HEADERS += \
    ../authentication_engine/user.h \
    ../authentication_engine/auth_manager.h \
    ../db/db_access.h \
    ../post/one_post.h \
    moderation_manager.h \
    moderator.h
