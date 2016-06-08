
#include "frame.h"


Frame::Frame(lzr::Frame& f)
{
    frame = f;
    find_paths();
}

Frame::~Frame()
{
    clear_paths();
}

int Frame::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return frame.size();
}

QVariant Frame::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role);

    if(!index.isValid())
        return QVariant();

    if(index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    QVariant v;
    v.setValue(paths[index.row()]);
    return v;
}

int Frame::columnCount(const QModelIndex& index) const
{
    Q_UNUSED(index);
    return 1;
}

QModelIndex Frame::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    if(column != 0)
        return QModelIndex();

    if(row >= rowCount())
        return QModelIndex();

    return createIndex(row, 0);
}

QModelIndex Frame::parent(const QModelIndex& index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

lzr::Point Frame::get_point(int i)
{
    if(i < 0 || i >= (int) frame.size())
        return lzr::Point();
    else
        return frame[i];
}

void Frame::find_paths()
{
    clear_paths();

    bool was_lit = false;
    size_t start = 0;
    for(size_t i = 0; i < frame.size(); i++)
    {
        lzr::Point& p = frame[i];

        if(p.is_lit() && !was_lit) //if the laser just turned on
        {
            start = i;
        }
        else if(p.is_blanked() && was_lit) //if the laser just turned off
        {
            //construct and save a new path
            paths.append(new Path(this, start, i - 1));
            //will never produce -1 becuase was_lit is initialized to
            //false at the beginning of looping
        }

        was_lit = p.is_lit();
    }

    //if a path was left open, close it
    if(was_lit)
        paths.append(new Path(this, start, frame.size() - 1));
}

void Frame::clear_paths()
{
    for(Path* p : paths)
        delete p;
    paths.clear();
}







/*
 * Paths
 */

Path::Path(QAbstractItemModel* frame, size_t s, size_t e)
{
    setSourceModel(frame);
    start = s;
    end = e;
}

int Path::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return end - start;
}

int Path::columnCount(const QModelIndex& index) const
{
    Q_UNUSED(index);
    return 1;
}

QVariant Path::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);

    QModelIndex source_index = mapToSource(index);

    if(!source_index.isValid())
        return QVariant();

    //lookup the point in the raw frame
    lzr::Point p = ((Frame*)sourceModel())->get_point(source_index.row());

    QVariant v;
    v.setValue(p);
    return v;
}

QModelIndex Path::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    if(column != 0)
        return QModelIndex();

    if(row >= rowCount())
        return QModelIndex();

    return createIndex(row, 0);
}

QModelIndex Path::parent(const QModelIndex& index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

QModelIndex Path::mapToSource(const QModelIndex& index) const
{
    if(!index.isValid())
        return QModelIndex();

    if(index.column() != 0)
        return QModelIndex();

    if(index.row() < 0 || index.row() >= rowCount())
        return QModelIndex();

    return createIndex(index.row() + start, 0);
}

QModelIndex Path::mapFromSource(const QModelIndex& index) const
{
    if(!index.isValid())
        return QModelIndex();

    if(index.column() != 0)
        return QModelIndex();

    if(index.row() < (int) start || index.row() > (int) end)
        return QModelIndex();

    return createIndex(index.row() - start, 0);
}
