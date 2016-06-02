
#pragma once

#include <QtWidgets>
#include <QDebug>



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
