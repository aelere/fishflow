#include <iostream>
#include "../authentication_engine/user.h"
#include "../db/db_access.h"
#include "../authentication_engine/auth_manager.h"
#include "upload.h"

using namespace std;

int main()
{

    // DOES NOT WORK RIGHT NOT BECAUSE currentUser under auth_manager() NEEDS TO BE INITIALIZED
    // THAT CLASS
    db_access myDb;
    auth_manager myManager(myDb);
    //upload my_uploads = upload(myDb, *myManager.getCurrentUser());
    //std::cout << myManager.getCurrentUser();
    //my_uploads.send("TITLE", "this is my fourth post");
    //std::cout << my_uploads.send("TITLE", "this is my second post");
//    std::vector<one_post> temp = my_uploads.findPosts();
//    for(int i = 0; i < temp.size(); i++){
//        std::cout << temp.at(i).return_postId() << std::endl;
//    }

//    my_uploads.report_post(1, 1, "test");

//    my_uploads.delete_post(2);
    //my_uploads.remove_post(3,1,"reason");
    return 0;
}
