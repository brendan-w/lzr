
#pragma once

#include "../frame.h"
#include "point.h"
#include "frameeditorstate.h"

#define PATH_END -1
#define PATH_START 0


class Path : public QGraphicsObject
{
    Q_OBJECT

public:
    Path(FrameEditorState* s, QModelIndex& i);
    Point* at(int i) const;
    Point* first() const;
    Point* last() const;
    size_t size() const;
    QModelIndex get_index();
    void from_LZR(lzr::Frame& path);
    lzr::Frame to_LZR() const;
    QRectF boundingRect() const;
    void setEnabled(bool enabled);
    void add_point(QPointF pos, int where = PATH_END);
    void select_all();
    void deselect_all();

signals:
    void changed(Path* path);

public slots:
    void point_changed();
    void remove_point(Point* point);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void own_point(Point* point);

    FrameEditorState* state; //only used for point construction
    QPersistentModelIndex index;
    QList<Point*> points;
};
