
#pragma once

#include <liblzr.hpp>
#include <QAbstractListModel>


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
    Frame(const lzr::Frame& frame);

    //QAbstractItemModel functions
    int rowCount(const QModelIndex& parent = QModelIndex()) const; //reports the number of paths in the frame
    int columnCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index, int role=Qt::DisplayRole) const; //retrieve one of the paths
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    QModelIndex index(int row, int column=0, const QModelIndex& parent=QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
    bool removeRow(int row, const QModelIndex& parent = QModelIndex());

    //custom Frame functions
    QModelIndex duplicate(const QModelIndex& index);
    QModelIndex add_path(const lzr::Frame& path=lzr::Frame());
    lzr::Frame get_path(const QModelIndex& index) const;
    bool set_path(const QModelIndex& index, lzr::Frame path);

private:
    lzr::FrameList paths; //partial laser frames, split by blanking jumps
};


