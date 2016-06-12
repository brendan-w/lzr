
#pragma once

#include <QPointF>
#include <QVector2D>


inline float distance_between_points(QPointF& a, QPointF& b)
{
    return QVector2D(a).distanceToPoint(QVector2D(b));
}
