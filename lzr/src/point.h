
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "liblzr.h"


class Point : public QGraphicsObject
{
    Q_OBJECT

public:
    Point(lzr::Point p);
    Point(QPointF p, QColor c);
    void init();

    QRectF boundingRect() const;
    void setColor(const QColor& c);
    QColor getColor() const;
    lzr::Point to_LZR() const;
    void compensate_for_view_transform();

signals:
    void changed();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

private:

    QColor color;
    bool hovered;
};
