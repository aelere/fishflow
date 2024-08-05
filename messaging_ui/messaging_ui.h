//#ifndef MESSAGING_UI_H
//#define MESSAGING_UI_H

//#include <QMainWindow>
//#include "../authentication_engine/user.h"
//#include "../db/db_access.h"
//#include "one_conversation.h"

//QT_BEGIN_NAMESPACE
//namespace Ui { class Messaging_UI; }
//QT_END_NAMESPACE

//class messaging_ui{
//    Q_OBJECT

//public:
//    messaging_ui(db_access& dbAccess, user& currentUser, QWidget *parent = nullptr);
//    ~messaging_ui();

//private slots:
//    void on_sendButton_clicked();
//    void on_conversationsList_itemClicked(QListWidgetItem *item);

//private:
//    void loadConversations();
//    void loadMessages(int user2_id);

//    Ui::Messaging_UI *ui;
//    db_access& m_dbAccess;
//    user& m_currentUser;
//    conversation m_conversation;
//};

//#endif // MESSAGING_UI_H
