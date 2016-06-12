
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
