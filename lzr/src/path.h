
#pragma once

#include "point.h"


class Path : public QGraphicsObject
{
    Q_OBJECT

public:
    Path(QModelIndex i, lzr::Frame frame);
    QRectF boundingRect() const;
    void add_point(Point* point, bool add_at_front=false);
    lzr::Frame to_LZR() const;
    QModelIndex get_index();
    Point* first();
    Point* last();
    void setEnabled(bool enabled);

signals:
    void changed(Path* path);

public slots:
    void point_changed();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void own_point(Point* point, int where);
    QList<Point*> points;
    QModelIndex index;
};
