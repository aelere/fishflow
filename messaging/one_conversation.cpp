#include "one_conversation.h"

one_conversation::one_conversation(int conversation_id,
                                   int user1_id,
                                   int user2_id){
    this->conversation_id = conversation_id;
    this->user1_id = user1_id;
    this->user2_id = user2_id;
}

int one_conversation::return_conversationId(){
    return conversation_id;
}

int one_conversation::return_user1Id(){
    return user1_id;
}

int one_conversation::return_user2Id(){
    return user2_id;
}
