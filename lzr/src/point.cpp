
#include "point.h"

#define POINT_RADIUS 10
#define POINT_DIAMETER (POINT_RADIUS * 2)
#define POINT_INNER_RADIUS 2
#define POINT_INNER_DIAMETER (POINT_INNER_RADIUS * 2)

#define HANDLE_COLOR 100, 100, 100, 255





Point::Point(lzr::Point p) : QGraphicsObject(0)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setPos(p.x, p.y);
    setColor(p.r, p.g, p.b, p.i);
    hovered = false;
}

Point::~Point()
{

}

void Point::setColor(int r, int g, int b, int a)
{
    color = QColor(r, g, b, a);
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
        QPointF newPos = value.toPointF();
        QRectF rect(-1.0, -1.0, 2.0, 2.0);

        if(!rect.contains(newPos))
        {
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }

        //moving the child should trigger a change in the parent
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
