
#pragma once

#include <QPointF>
#include <QRectF>
#include <QVector2D>


typedef struct {
    double actual;     // seconds since show start
    double normalized; // 0-1 normalized for the clip in question
} Time;


const QRectF frame_bounds(-1.0, -1.0, 2.0, 2.0);

inline float distance_between_points(const QPointF& a, const QPointF& b)
{
    return QVector2D(a).distanceToPoint(QVector2D(b));
}

inline bool perpendicular_intersection(const QPointF& a,
                                       const QPointF& b,
                                       const QPointF& x,
                                       QPointF& intersection)
{
    QVector2D ab(b - a);
    QVector2D ax(x - a);

    QVector2D i(ab);
    i.normalize();
    i *= QVector2D::dotProduct(i, ax);
    i += QVector2D(a);

    intersection = i.toPointF();

    QVector2D ai(intersection - a);
    float dot = QVector2D::dotProduct(ab, ai);
    bool inside = (dot > 0.0) && (ai.lengthSquared() < ab.lengthSquared());

    return inside;
}

QPointF constrain_and_snap(QPointF p, bool snap=false, int divisions=1);
