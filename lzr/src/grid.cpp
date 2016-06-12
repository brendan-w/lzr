
#include "grid.h"
#include <QDebug>


static const QRectF frame_bounds(-1.0, -1.0, 2.0, 2.0);


Grid::Grid(QGraphicsItem* parent) :
    QGraphicsRectItem(parent)
{
    divisions = 8;
    snapping = false;
    setPen(QPen(QColor(60, 60, 60), 0));
    setRect(frame_bounds);
}

void Grid::set_divisions(int d)
{
    divisions = d;
    update();
}

void Grid::set_snapping(bool snap)
{
    snapping = snap;
}

QPointF Grid::constrain_and_maybe_snap(QPointF p)
{
    p = constrain_to_frame(p);
    if(snapping)
        p = snap_to_grid(p);
    return p;
}

//constrains the given point to the area of LZR's frame
QPointF Grid::constrain_to_frame(QPointF p)
{
    p.setX(qMin(frame_bounds.right(), qMax(p.x(), frame_bounds.left())));
    p.setY(qMin(frame_bounds.bottom(), qMax(p.y(), frame_bounds.top())));
    return p;
}

QPointF Grid::snap_to_grid(const QPointF& point)
{
    return QPointF(single_axis_snap(point.x()),
                   single_axis_snap(point.y()));
}

float Grid::single_axis_snap(float x)
{
    float division_size = (float) 2 / divisions;
    float remainder = fmod(x, division_size);
    float norm_pos = remainder / division_size;

    if(fabs(norm_pos) < 0.5)
    {
        //the value is on the inside of the halfway mark
        return x - remainder;
    }
    else if(norm_pos >= 0.5)
    {
        //outside of the halfway mark, positive quadrant
        return x + (division_size - remainder);
    }
    else if(norm_pos <= -0.5)
    {
        //outside of the halfway mark, negative quadrant
        return x - (division_size + remainder);
    }

    return 0; //else, something went wrong...
}


void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //paint the outer rect
    QGraphicsRectItem::paint(painter, option, widget);

    QRectF r = rect();
    QLineF line;

    for(int i = 1; i < divisions; i++)
    {
        //normalized
        double n = (double) i / divisions;

        //vertical
        double x = (r.width() * n) + r.left();
        line.setLine(x, r.top(), x, r.bottom());
        painter->drawLine(line); //need to use QLineF to get floating point

        //horizontal
        double y = (r.height() * n) + r.top();
        line.setLine(r.left(), y, r.right(), y);
        painter->drawLine(line);
    }
}
