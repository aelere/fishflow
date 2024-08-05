#include <QAbstractItemView>
#include <QMouseEvent>

class MyItemView : public QAbstractItemView
{
public:
    MyItemView(QWidget *parent = nullptr) : QAbstractItemView(parent) {}

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid()) {
            event->ignore();
            return;
        }
        QAbstractItemView::mouseReleaseEvent(event);
    }
};

