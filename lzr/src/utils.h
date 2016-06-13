
#pragma once

#include <QPointF>
#include <QRectF>
#include <QVector2D>


const QRectF frame_bounds(-1.0, -1.0, 2.0, 2.0);

inline float distance_between_points(QPointF& a, QPointF& b)
{
    return QVector2D(a).distanceToPoint(QVector2D(b));
}

QPointF constrain_and_snap(QPointF p, bool snap=false, int divisions=1);
