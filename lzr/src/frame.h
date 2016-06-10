
#pragma once

#include "liblzr.h"
#include <QAbstractListModel>
#include <QAbstractProxyModel>


Q_DECLARE_METATYPE(lzr::Frame);

/*
 * Main frame model
 *
 * This model exposes the vector data as a list of paths
 * paths are contiguous, lit points, and are handled as
 * individual entities by the UI.
 */
class Frame : public QAbstractListModel
{
    Q_OBJECT

public:
    Frame(lzr::Frame& frame);
    int rowCount(const QModelIndex &parent = QModelIndex()) const; //reports the number of paths in the frame
    int columnCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const; //retrieve one of the paths
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QModelIndex index(int row, int column=0, const QModelIndex& parent=QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;

private:
    QList<lzr::Frame> paths; //partial laser frame, split by blanking jumps
};


