
#pragma once

#include <QRectF>
#include <QPointF>
#include <QVector2D>

static const QRectF frame_bounds(-1.0, -1.0, 2.0, 2.0);


//constrains the given point to the area of LZR's frame
inline QPointF constrain_to_frame(QPointF p)
{
    p.setX(qMin(frame_bounds.right(), qMax(p.x(), frame_bounds.left())));
    p.setY(qMin(frame_bounds.bottom(), qMax(p.y(), frame_bounds.top())));
    return p;
}

inline float distance_between_points(QPointF& a, QPointF& b)
{
    return QVector2D(a).distanceToPoint(QVector2D(b));
}
