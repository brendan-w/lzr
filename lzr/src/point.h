
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "liblzr.h"
#include "grid.h"


class Point : public QGraphicsObject
{
    Q_OBJECT

public:
    Point(lzr::Point p, Grid* g);
    Point(QPointF p, QColor c, Grid* g);
    void init();

    QRectF boundingRect() const;
    void setColor(const QColor& c);
    QColor getColor() const;
    lzr::Point to_LZR() const;

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
    Grid* grid; //used for snapping
};
