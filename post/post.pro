TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
#CONFIG -= qt

QT += core
QT += sql

SOURCES += \
    main.cpp \
    ../db/db_access.cpp \
    ../authentication_engine/user.cpp \
    ../authentication_engine/auth_manager.cpp \
    one_post.cpp \
    upload.cpp

HEADERS += \
    ../db/db_access.h \
    ../authentication_engine/user.h \
    ../authentication_engine/auth_manager.h \
    one_post.h \
    upload.h

