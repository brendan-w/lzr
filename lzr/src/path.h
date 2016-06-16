
#pragma once

#include "point.h"
#include "frame.h"
#include "frameeditorstate.h"


class Path : public QGraphicsObject
{
    Q_OBJECT

public:
    Path(FrameEditorState* s, QModelIndex& i);
    Point* first();
    Point* last();
    size_t size();
    void from_LZR(lzr::Frame& path);
    lzr::Frame to_LZR() const;
    QRectF boundingRect() const;
    void add_point(Point* point, bool add_at_front=false);
    QModelIndex get_index();
    void setEnabled(bool enabled);

signals:
    void changed(Path* path);

public slots:
    void point_changed();
    void remove_point(Point* point);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void own_point(Point* point, int where);

    FrameEditorState* state;
    QPersistentModelIndex index;
    QList<Point*> points;
};
