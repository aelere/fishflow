#ifndef CHECKABLECOMBOBOX_H
#define CHECKABLECOMBOBOX_H

#include <QComboBox>
#include <QMouseEvent>

// Class: checkableComboBox
// ------------------------
// This class extends the functionality of QComboBox to provide a checkable combobox.
// It allows the user to select multiple items from the dropdown list by enabling check boxes.
// This is accomplished by overriding the hidePopup() method.

class checkableComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit checkableComboBox(QWidget *parent = nullptr);

protected:
    // Overriding the hidePopup method from QComboBox
    void hidePopup() override;
};

#endif // CHECKABLECOMBOBOX_H
