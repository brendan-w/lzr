
#include "frame.h"


Frame::Frame(lzr::Frame& f)
{
    frame = f;
}

int Frame::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return frame.size();
}

QVariant Frame::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= (int) frame.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        QVariant r;
        r.setValue(frame[index.row()]);
        return r;
    }
    else
        return QVariant();
}
