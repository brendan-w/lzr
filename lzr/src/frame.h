
#pragma once

#include "liblzr.h"
#include <QAbstractListModel>
#include <QAbstractProxyModel>

Q_DECLARE_METATYPE(lzr::Point);

/*

 */
class Path : public QAbstractProxyModel
{
    Q_OBJECT

public:
    void setPathNumber(int n);

private:
    int n;
};


/*
 * main frame model, representing a list of points
 */
class Frame : public QAbstractListModel
{
    Q_OBJECT

public:
    Frame(lzr::Frame& f);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    lzr::Frame frame;
};
