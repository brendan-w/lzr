
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
    if(!index.isValid())
        return QVariant();

    if(index.row() >= rowCount())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        QVariant r;
        r.setValue(frame[index.row()]);
        return r;
    }
    else
    {
        return QVariant();
    }
}




/*
 * Paths
 */

int Path::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if(!sourceModel())
        return 0;

    //TODO
    return 0;
}

QVariant Path::data(const QModelIndex &index, int role) const
{
    //TODO
    return QVariant();
}

void Path::setPathNumber(int _n)
{
    n = _n;
}
