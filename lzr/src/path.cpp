
#include "path.h"


Path::Path(FrameEditorState* s, QModelIndex& i) : QGraphicsObject(0),
                                                  state(s),
                                                  index(i)
{
    setEnabled(false); //never handle user events

    //convert laser path into actual point items
    lzr::Frame path = index.data().value<lzr::Frame>();
    from_LZR(path);
}

Point* Path::at(int i) const
{
    return points[i];
}

Point* Path::first() const
{
    return points.front();
}

Point* Path::last() const
{
    return points.back();
}

size_t Path::size() const
{
    return points.size();
}

lzr::Frame Path::to_LZR() const
{
    lzr::Frame frame;

    foreach(Point* point, points)
    {
        frame.add(point->to_LZR());
    }

    return frame;
}

void Path::from_LZR(lzr::Frame& path)
{
    foreach(Point* point, points)
    {
        delete point;
    }

    points.clear();

    for(lzr::Point lzr_point : path)
    {
        own_point(new Point(state, lzr_point), points.size());
    }
}

void Path::setEnabled(bool enabled)
{
    foreach(Point* point, points)
    {
        point->setEnabled(enabled);
        point->setVisible(enabled);
    }

    //always show the enabled paths on top of the disabled ones
    setZValue(enabled ? 1 : 0);
}

QRectF Path::boundingRect() const
{
    return QRectF(-1,-1,2,2); //the whole screen/grid
}

void Path::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    for(int i = 0; i < points.size() - 1; i++)
    {
        const Point* p1 = points[i];
        const Point* p2 = points[i+1];
        QLineF line(p1->x(), p1->y(), p2->x(), p2->y());
        painter->setPen(QPen(p2->get_color(), 0));
        painter->drawLine(line);
    }
}

void Path::add_point(QPointF pos, int where)
{
    Point* point = new Point(state, pos, state->color);
    point->setEnabled(true);
    point->setVisible(true);

    if(where == PATH_END)
        own_point(point, points.size());
    else
        own_point(point, where);

    //since this point was added at the back, and colors are stored on
    //the second point, we need to copy this point's color to the next point
    if(where == PATH_START && size() > 1)
        points[1]->set_color(points[0]->get_color());

    emit changed(this);
}

QModelIndex Path::get_index()
{
    return index;
}

void Path::select_all()
{
    foreach(Point* point, points)
    {
        point->setSelected(true);
    }
}

void Path::deselect_all()
{
    foreach(Point* point, points)
    {
        point->setSelected(false);
    }
}

void Path::own_point(Point* point, int where)
{
    points.insert(where, point);
    point->setParentItem(this);
    connect(point, SIGNAL(changed()),
            this, SLOT(point_changed()));
    connect(point, SIGNAL(remove_point(Point*)),
            this, SLOT(remove_point(Point*)));
}



/*
 * Slots
 */

void Path::point_changed()
{
    emit changed(this);
}

void Path::remove_point(Point* point)
{
    points.removeAll(point);
    delete point;

    emit changed(this);
}
