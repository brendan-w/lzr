
#pragma once

#include "point.h"


class Path : public QGraphicsObject
{
    Q_OBJECT

public:
    Path(QModelIndex i, lzr::Frame frame);
    QRectF boundingRect() const;
    void add_point(Point* point);
    lzr::Frame to_LZR() const;
    QModelIndex get_index();

signals:
    void changed(Path* path);

public slots:
    void point_changed();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void own_point(Point* point);
    QModelIndex index;
};
