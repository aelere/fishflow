#include <iostream>
#include "../authentication_engine/user.h"
#include "../db/db_access.h"
#include "../authentication_engine/auth_manager.h"
#include "conversation.h"
#include "one_message.h"
#include "one_conversation.h"

using namespace std;

int main()
{
    db_access myDb;
    auth_manager myManager(myDb);
    myManager.authenticate("Ado", "adobado555");
    //myManager.createUser("Ado", "Andria", "Saneblidze",
    //                         "adobado@gmail.com", QDate::currentDate().toString(), "adobado555", 8293482194);

//    conversation my_conversation = conversation(myDb, *myManager.getCurrentUser());
//    my_conversation.create_conversation(3);
//    my_conversation.send_message(3, "hello fellow user part 2");
//    std::vector<one_message> temp1 = my_conversation.all_messages(3);
//    std::vector<one_conversation> temp2 = my_conversation.all_conversations();

    return 0;
}
