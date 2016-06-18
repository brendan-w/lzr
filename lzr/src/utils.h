
#pragma once

#include <QPointF>
#include <QRectF>
#include <QVector2D>


const QRectF frame_bounds(-1.0, -1.0, 2.0, 2.0);

inline float distance_between_points(const QPointF& a, const QPointF& b)
{
    return QVector2D(a).distanceToPoint(QVector2D(b));
}

inline QPointF perpendicular_intersection(const QPointF& a, const QPointF& b, const QPointF& x)
{
    QVector2D ab(b - a);
    QVector2D ax(x - a);

    QVector2D intersection(ab);
    intersection.normalize();
    intersection *= QVector2D::dotProduct(intersection, ax);
    intersection += QVector2D(a);

    return intersection.toPointF();
}

QPointF constrain_and_snap(QPointF p, bool snap=false, int divisions=1);
