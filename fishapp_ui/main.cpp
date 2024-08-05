#include "mainwindow.h"
#include "main_feed.h"
#include "messaging.h"
#include "conversation.h"
#include "qapplication.h"
#include <QtCore>

//#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir bin(QCoreApplication::applicationDirPath());

        // Set working directory
        #if defined(Q_OS_MACOS)
            bin.cdUp();
            bin.cdUp();
            bin.cdUp();
            QDir::setCurrent(bin.absolutePath());
        #endif


    db_access myDb;
    auth_manager myAuth(myDb);
    moderation_manager myMod(myDb);
    MainWindow w(myDb, myAuth, myMod);
    //main_feed w;
    //messaging w;
    //conversation w;

    w.show();
    return a.exec();

}
