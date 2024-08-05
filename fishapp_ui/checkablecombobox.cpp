#include "checkablecombobox.h"
#include "qabstractitemview.h"
#include "qapplication.h"
#include "qcoreevent.h"
#include "qevent.h"

checkableComboBox::checkableComboBox(QWidget *parent)
    : QComboBox(parent)
{
}

void checkableComboBox::hidePopup()
{
    if (view()->currentIndex().isValid()) {
        QPoint globalPos = mapToGlobal(QPoint(0, 0));
        QRect comboRect(globalPos.x(), globalPos.y(), width(), height());

        if (!(QApplication::mouseButtons() & Qt::LeftButton) || !comboRect.contains(QCursor::pos()))
        {
            // Do nothing if the left mouse button is not pressed or the mouse is not inside the combobox
        }else{
            // Call the base QComboBox hidePopup() method to hide the popup
            QComboBox::hidePopup();
        }
    }
}



