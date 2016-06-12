
#include "point.h"


#define POINT_RADIUS 10
#define POINT_DIAMETER (POINT_RADIUS * 2)
#define POINT_INNER_RADIUS 2
#define POINT_INNER_DIAMETER (POINT_INNER_RADIUS * 2)

#define HANDLE_COLOR 100, 100, 100, 255





Point::Point(lzr::Point p) : QGraphicsObject(0)
{
    init();
    setPos(p.x, p.y);
    color = QColor(p.r, p.g, p.b, p.i);
}

Point::Point(QPointF p, QColor c) : QGraphicsObject(0)
{
    init();
    setPos(p);
    color = c;
}

void Point::init()
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setAcceptHoverEvents(true);
    hovered = false;
}

void Point::setColor(const QColor& c)
{
    color = c;
}

QColor Point::getColor() const
{
    return color;
}

lzr::Point Point::to_LZR() const
{
    return lzr::Point(x(),
                      y(),
                      color.red(),
                      color.green(),
                      color.blue(),
                      color.alpha());
}

QRectF Point::boundingRect() const
{
    return QRectF(-POINT_RADIUS, -POINT_RADIUS, POINT_DIAMETER, POINT_DIAMETER);
}


void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(hovered)
    {
        painter->setPen(QColor(HANDLE_COLOR));
        painter->drawEllipse(-POINT_RADIUS, -POINT_RADIUS, POINT_DIAMETER, POINT_DIAMETER);
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(color);
    painter->drawEllipse(-POINT_INNER_RADIUS, -POINT_INNER_RADIUS, POINT_INNER_DIAMETER, POINT_INNER_DIAMETER);
}


QVariant Point::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(scene() && change == ItemPositionChange)
    {
        // Keep the item inside the scene rect.
        QPointF pos = value.toPointF();//constrain_to_frame(value.toPointF());

        //if the point changed, return the changed point
        if(value.toPointF() != pos)
        {
            return pos;
        }
    }
    else if(scene() && change == ItemPositionHasChanged)
    {
        //moving the child should trigger a change in the parent,
        //to redraw the connecting lines
        emit changed();
    }

    //call default handler
    return QGraphicsItem::itemChange(change, value);
}


void Point::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    hovered = true;
    return QGraphicsItem::hoverEnterEvent(event);
}


void Point::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    hovered = false;
    return QGraphicsItem::hoverEnterEvent(event);
}
