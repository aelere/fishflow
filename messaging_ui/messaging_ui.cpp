//#include "messaging_ui.h"
//#include "ui_messaging_ui.h"
//#include <QListWidgetItem>

//messaging_ui::messaging_ui() {
//    delete ui;
//}


//messaging_ui::messaging_ui(db_access& dbAccess, user& currentUser, QWidget *parent)
//    : QMainWindow(parent)
//    , ui(new Ui::Messaging_UI)
//    , m_dbAccess(dbAccess)
//    , m_currentUser(currentUser)
//    , m_conversation(dbAccess, currentUser){
//    ui->setupUi(this);
//    loadConversations();
//}

//void messaging_ui::loadConversations(){
//    auto conversations = m_conversation.all_conversations();

//    for (const auto& conv : conversations) {
//        int user2_id = (conv.return_user1Id() == m_currentUser.getId()) ? conv.return_user2Id() : conv.return_user1Id();
//        QListWidgetItem *item = new QListWidgetItem(QString("Conversation with User %1").arg(user2_id), ui->conversationsList);
//        item->setData(Qt::UserRole, user2_id);
//    }

//}

//void messaging_ui::loadMessages(int user2_id){
//    ui->messagesDisplay->clear();
//    auto messages = m_conversation.all_messages(user2_id);

//    for (const auto& msg : messages) {
//        ui->messagesDisplay->appendPlainText(QString("[%1] User %2: %3").arg(QString::fromStdString(msg.return_timestamp()), QString::number(msg.return_senderId()), QString::fromStdString(msg.return_content())));
//    }
//}

//void messaging_ui::on_sendButton_clicked(){
//    int user2_id = ui->conversationsList->currentItem()->data(Qt::UserRole).toInt();
//    QString messageContent = ui->messageInput->text();

//    if (m_conversation.send_message(user2_id, messageContent.toStdString())) {
//        loadMessages(user2_id);
//        ui->messageInput->clear();
//    }
//}

//void messaging_ui::on_conversationsList_itemClicked(QListWidgetItem *item){
//    int user2_id = item->data(Qt::UserRole).toInt();
//    loadMessages(user2_id);
//}
