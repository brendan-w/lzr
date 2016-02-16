
#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QDebug>


#define POINT_RADIUS 10
#define POINT_DIAMETER (POINT_RADIUS * 2)
#define POINT_INNER_RADIUS 3
#define POINT_INNER_DIAMETER (POINT_INNER_RADIUS * 2)

#define HANDLE_COLOR 100, 100, 100, 255


class Point : public QGraphicsItem
{
public:
    Point(QGraphicsItem *parent = 0);
    ~Point();

    QRectF boundingRect() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

private:
    bool hovered;
};
