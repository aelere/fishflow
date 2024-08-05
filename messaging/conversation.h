#ifndef CONVERSATION_H
#define CONVERSATION_H

#include "../authentication_engine/auth_manager.h"
#include "one_conversation.h"
#include "one_message.h"


class conversation
{
public:
    conversation(auth_manager& userAuthentication); //db_access& dbAccess, user& userAccess);
    bool create_conversation(int user2_id);
    bool delete_conversation(int user2_id);
    std::vector<one_conversation> all_conversations();
    std::vector<one_message> all_messages(int user2_id);
    bool send_message(int user2_id, std::string content);

private:
//    db_access& m_dbAccess;
//    user& myUser;
    auth_manager& myAuthentication;
};

#endif // CONVERSATION_H
