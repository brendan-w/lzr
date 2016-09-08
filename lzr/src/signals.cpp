
#include "signals.h"



CurvePoint* CurveSignal::addPoint(QPointF pos)
{
    CurvePoint* p = new CurvePoint();
    p->x = input_clamp(pos.x());
    p->y = output_clamp(pos.y()); //constrain the range of this function

    //TODO: binary insert
    std::sort(points.begin(), points.end(),
              [](CurvePoint* a, CurvePoint* b) { return a->x > b->x; });

    return p;
}

bool CurveSignal::removePoint(CurvePoint* p)
{
    //there must always be at least 2 points in the curve
    if(points.size() <= 2)
        return false;

    points.removeAll(p);
    delete p;
    return true;
}

void CurveSignal::movePoint(CurvePoint* p, QPointF pos)
{
    p->x = input_clamp(pos.x());
    p->y = output_clamp(pos.y()); //constrain the range of this function

    //re-sort
    //TODO: since only one element will be out of place, this can be done linearly
    std::sort(points.begin(), points.end(),
              [](CurvePoint* a, CurvePoint* b) { return a->x > b->x; });
}
