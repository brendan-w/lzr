

#include "clip.h"


Clip::Clip()
{
    //new clips are always populated with a base frame generator effect
    effects.push_back(new FrameEffect());
}

Clip::~Clip()
{
    for(Effect* e : effects)
    {
        delete e;
    }
}

lzr::Frame Clip::run(Time& t) {
    lzr::Frame frame;

    for(Effect* e : effects)
    {
        if(e->active)
            e->run(frame, t);
    }

    return frame;
}

/*
 * Qt Model interface
 */

int Clip::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return effects.size();
}

QVariant Clip::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role);
    QVariant v;
    v.setValue(get_effect(index));
    return v;
}

QModelIndex Clip::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    if(column != 0)
        return QModelIndex();

    if(row < 0 || row >= rowCount())
        return QModelIndex();

    return createIndex(row, 0);
}

Effect* Clip::get_effect(const QModelIndex& index) const
{
    if(!index.isValid())
        return NULL;

    if(index.row() < 0 || index.row() >= rowCount())
        return NULL;

    return effects[index.row()];
}
