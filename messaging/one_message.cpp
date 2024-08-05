#include "one_message.h"

one_message::one_message(int message_id,
                         int conversation_id,
                         int sender_id,
                         std::string content,
                         std::string timestamp){
    this->message_id = message_id;
    this->conversation_id = conversation_id;
    this->sender_id = sender_id;
    this->content = content;
    this->timestamp = timestamp;
}

int one_message::return_messageId(){
    return message_id;
}

int one_message::return_conversationId(){
    return conversation_id;
}

int one_message::return_senderId(){
    return sender_id;
}

std::string one_message::return_content(){
    return content;
}

std::string one_message::return_timestamp(){
    return timestamp;
}
