TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
QT += core
QT += sql

SOURCES += \
        auth_manager.cpp \
        main.cpp \
        user.cpp \
        ../db/db_access.cpp \
        ../moderation/moderation_manager.cpp \
        ../moderation/moderator.cpp \

HEADERS += \
    auth_manager.h \
    user.h \
    ../db/db_access.h \
    ../moderation/moderation_manager.h \
    ../moderation/moderator.h \
