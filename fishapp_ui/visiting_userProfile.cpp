#include "visiting_user_profile.h"

visiting_user_profile::visiting_user_profile(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant visiting_user_profile::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex visiting_user_profile::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex visiting_user_profile::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int visiting_user_profile::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int visiting_user_profile::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant visiting_user_profile::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
