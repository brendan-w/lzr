
#pragma once

#include "liblzr.h"
#include <QAbstractListModel>
#include <QAbstractProxyModel>


/*

 */
class Path : public QAbstractProxyModel
{
    Q_OBJECT

public:
    Path(QAbstractItemModel* frame, size_t s, size_t e);
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const; //retrieve one of the points
    QModelIndex index(int row, int column=0, const QModelIndex& parent=QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
    QModelIndex mapToSource(const QModelIndex& index) const;
    QModelIndex mapFromSource(const QModelIndex& index) const;

private:
    size_t start; //point indices in the raw lzr::Frame
    size_t end;
};


/*
 * Main frame model
 *
 * This model exposes the vector data as a list of paths (proxy models)
 * paths are contiguous, lit points, and are handled as
 * individual entities by the UI. This model handles all
 * abstraction of paths vs the raw list of points in the frame.
 */
class Frame : public QAbstractListModel
{
    Q_OBJECT

public:
    Frame(lzr::Frame& f);
    ~Frame();
    int rowCount(const QModelIndex &parent = QModelIndex()) const; //reports the number of paths in the frame
    int columnCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const; //retrieve one of the paths
    QModelIndex index(int row, int column=0, const QModelIndex& parent=QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;

    lzr::Point get_point(int i);

private:
    lzr::Frame frame; //the raw frame of points
    QList<Path*> paths; //list of proxy models representing sequences of points

    void find_paths();
    void clear_paths();
};


Q_DECLARE_METATYPE(lzr::Point);
Q_DECLARE_METATYPE(Path*);
