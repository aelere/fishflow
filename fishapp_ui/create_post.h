#ifndef CREATE_POST_H
#define CREATE_POST_H

#include "ui_create_post.h"
#include "../authentication_engine/auth_manager.h"
#include "../post/upload.h"

#include <QDialog>


namespace Ui {
class create_post;
}
// Class: create_post
// ------------------------
// This class represents a dialog window for creating a post. It allows users to enter a post title
// and content, select interests associated with the post, and submit or cancel the post creation.
class create_post : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    create_post(auth_manager &myManager, QWidget *parent);
    // Destructor
    ~create_post();

signals:
    void postCreated();

private slots:
    // Slot triggered when the post button is clicked
    void on_postButton_clicked();

    // Slot triggered when the cancel button is clicked
    void on_cancelButton_clicked();

    // Slot triggered when an item in interestsBox is activated
    void on_interestsBox_activated(int index);

    // Slot triggered when the add button is clicked
    void on_addButton_clicked();

private:
    Ui::create_post *ui;
    auth_manager* myManager;
    upload myUpload;

    QStringList interests = {
        "tropical_fish",
        "freshwater_fish",
        "marine_fish",
        "aquarium_maintenance",
        "fish_tank_setup",
        "fish_breeding",
        "fish_conservation",
        "aquascaping",
        "fish_photography",
        "aquatic_plants",
        "reef_tank",
        "fish_community",
        "fish_videos",
        "public_aquariums",
        "saltwater_tank"
    };
};

#endif // CREATE_POST_H

