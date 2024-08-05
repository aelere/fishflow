#include <iostream>
#include <QApplication>
#include "messaging_ui.h"
#include "../authentication_engine/user.h"
#include "../db/db_access.h"
#include "../authentication_engine/auth_manager.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    // Create an instance of db_access and user classes
    db_access db;
    user currentUser(db);

    // Create and show the Messaging_UI window
    MessagingUI w(db, currentUser);
    w.show();

    return a.exec();
}
