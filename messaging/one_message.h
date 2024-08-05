#ifndef ONE_MESSAGE_H
#define ONE_MESSAGE_H
#include "string"


class one_message
{
public:
    one_message(int message_id,
                int conversation_id,
                int sender_id,
                std::string content,
                std::string timestamp);
    int return_messageId();
    int return_conversationId();
    int return_senderId();
    std::string return_content();
    std::string return_timestamp();
private:
    int message_id;
    int conversation_id;
    int sender_id;
    std::string content;
    std::string timestamp;
};

#endif // ONE_MESSAGE_H
