
#include "point.h"

#define POINT_RADIUS 10
#define POINT_DIAMETER (POINT_RADIUS * 2)
#define POINT_INNER_RADIUS 2
#define POINT_INNER_DIAMETER (POINT_INNER_RADIUS * 2)

#define HANDLE_COLOR 100, 100, 100, 255




Point::Point(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setPos(0, 0);
    hovered = false;
}

Point::~Point()
{

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
    painter->setBrush(Qt::green);
    painter->drawEllipse(-POINT_INNER_RADIUS, -POINT_INNER_RADIUS, POINT_INNER_DIAMETER, POINT_INNER_DIAMETER);
}


QVariant Point::itemChange(GraphicsItemChange change, const QVariant &value)
{
    // qDebug() << "change" << change;

    if(scene() && change == ItemPositionChange)
    {
        // qDebug() << "pos (" << pos().x() << ", " << pos().y() << ")";

        // Keep the item inside the scene rect.
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();

        if(!rect.contains(newPos))
        {
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }
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
