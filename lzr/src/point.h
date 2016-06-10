
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "liblzr.h"



class Point : public QGraphicsObject
{
    Q_OBJECT

public:
    Point(lzr::Point p);
    ~Point();

    QRectF boundingRect() const;
    void setColor(int r, int g, int b, int a);
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
};
