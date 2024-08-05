#include "one_post.h"

one_post::one_post(int post_id, int author_id,
                   QString title, QString content, QString timestamp){
    this->post_id = post_id;
    this->author_id = author_id;
    this->title = title;
    this->content = content;
    this->timestamp = timestamp;
}

int one_post::return_postId(){
    return post_id;
}

int one_post::return_authorId() const{
    return author_id;
}

QString one_post::return_title(){
    return title;
}

QString one_post::return_content(){
    return content;
}

QString one_post::return_timestamp(){
    return timestamp;
}
