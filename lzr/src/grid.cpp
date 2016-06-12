
#include "grid.h"
#include <QDebug>


Grid::Grid(QGraphicsItem* parent) :
    QGraphicsRectItem(parent)
{
    divisions = 8;
    setPen(QPen(QColor(60, 60, 60), 0));
    setRect(-1.0, -1.0, 2.0, 2.0);
}

void Grid::set_divisions(int d)
{
    divisions = d;
    update();
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

    return 0; //else, something when wrong...
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
