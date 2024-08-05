#ifndef ONE_POST_H
#define ONE_POST_H
#include <QString>

// Class: one_post
// ---------------------
// The one_post class represents a single post in the system.
// It contains information such as post ID, author ID, title, content, and timestamp.

class one_post
{
public:
    one_post(int post_id, int author_id,
             QString title, QString content, QString timestamp);

    int return_postId();
    int return_authorId() const;
    QString return_title();
    QString return_content();
    QString return_timestamp();

private:
    int post_id;
    int author_id;
    QString title;
    QString content;
    QString timestamp;
};

#endif // ONE_POST_H
