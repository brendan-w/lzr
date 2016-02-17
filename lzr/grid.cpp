
#include "grid.h"


Grid::Grid(QGraphicsItem* parent) :
    QGraphicsRectItem(parent)
{
    divisions = 1;
    setPen(QPen(QColor(100, 100, 100), 0));
    setRect(-1.0, -1.0, 2.0, 2.0);
}

Grid::~Grid()
{

}

void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
}
