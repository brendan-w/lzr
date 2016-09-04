
#include "grid.h"
#include "../utils.h"


Grid::Grid(FrameEditorState* s) : QGraphicsRectItem(0)
{
    state = s;
    setPen(QPen(QColor(60, 60, 60), 0));
    setRect(frame_bounds);
    setEnabled(false); //never respond to events
}

void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //paint the outer rect
    QGraphicsRectItem::paint(painter, option, widget);

    QRectF r = rect();
    QLineF line;

    for(int i = 1; i < state->grid_divisions; i++)
    {
        //normalized
        double n = (double) i / state->grid_divisions;

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
