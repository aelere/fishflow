#ifndef ONE_CONVERSATION_H
#define ONE_CONVERSATION_H


class one_conversation
{
public:
    one_conversation(int conversation_id,
                     int user1_id,
                     int user2_id);
    int return_conversationId();
    int return_user1Id();
    int return_user2Id();


private:
    int conversation_id;
    int user1_id;
    int user2_id;
};

#endif // ONE_CONVERSATION_H
