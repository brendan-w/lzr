
#pragma once

#include <QtWidgets>
#include <QDebug>
#include "liblzr.h"
#include "frameeditorstate.h"


class Point : public QGraphicsObject
{
    Q_OBJECT

public:
    Point(FrameEditorState* state, lzr::Point p);
    Point(FrameEditorState* state, QPointF p, QColor c);
    void init();

    QRectF boundingRect() const;
    void set_color(const QColor& c);
    QColor get_color() const;
    lzr::Point to_LZR() const;
    void remove();

signals:
    void changed();
    void remove_point(Point* point);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

private:
    QColor color;
    bool hovered;
    FrameEditorState* state;
};
