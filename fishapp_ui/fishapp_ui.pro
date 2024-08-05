QT += core gui
QT += core
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


SOURCES += \
    change_profile.cpp \
    checkablecombobox.cpp \
    #conversation.cpp \
    create_post.cpp \
    createuser.cpp \
    hashtag_feed.cpp \
    hashtag_search.cpp \
    login_moderator.cpp \
    login_user.cpp \
    main.cpp \
    main_feed.cpp \
    mainwindow.cpp \
    #messaging.cpp \
    moderator_feed.cpp \
    report_submission.cpp \
    user_interest.cpp \
    user_profile.cpp \
    ../db/db_access.cpp \
    ../authentication_engine/user.cpp \
    ../authentication_engine/auth_manager.cpp \
    ../moderation/moderator.cpp \
    ../moderation/moderation_manager.cpp \
    ../feed/user_feed.cpp \
    ../post/one_post.cpp \
    ../post/upload.cpp \
    user_settings.cpp \
    visiting_user_profile.cpp


HEADERS += \
    change_profile.h \
    checkablecombobox.h \
    #conversation.h \
    create_post.h \
    createuser.h \
    hashtag_feed.h \
    hashtag_search.h \
    login_moderator.h \
    login_user.h \
    mainwindow.h \
    main_feed.h \
    #messaging.h \
    moderator_feed.h \
    report_submission.h \
    user_interest.h \
    user_profile.h \
    ../db/db_access.h \
    ../authentication_engine/user.h \
    ../authentication_engine/auth_manager.h \
    ../moderation/moderator.h \
    ../moderation/moderation_manager.h \
    ../feed/user_feed.h \
    ../post/one_post.h \
    ../post/upload.h \
    user_settings.h \
    visiting_user_profile.h


FORMS += \
    admin_main_feed.ui \
    change_profile.ui \
    conversations.ui \
    create_post.ui \
    dialog.ui \
    hashtag_feed.ui \
    hashtag_search.ui \
    login_moderator.ui \
    login_user.ui \
    main_feed.ui \
    mainwindow.ui \
    messaging.ui \
    createuser.ui \
    moderator_feed.ui \
    report_submission.ui \
    settings.ui \
    user_interest.ui \
    user_profile.ui \
    user_settings.ui \
    visiting_user_profile.ui



