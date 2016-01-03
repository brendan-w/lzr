
#include "optimizer.h"

namespace lzr {



//handy test for whether the path has been inverted
#define INVERTED (b < a)



Optimizer_Path::Optimizer_Path(size_t _a, size_t _b, const std::vector<Optimizer_Point>* _points)
{
    a = _a;
    b = _b;
    points = _points;
}


size_t Optimizer_Path::size()
{
    if(INVERTED)
        return a - b + 1;
    else
        return b - a + 1;
}


//swaps A and B
void Optimizer_Path::invert()
{
    size_t temp = a;
    a = b;
    b = temp;
}


double Optimizer_Path::entrance_angle()
{
    if(size() < 2)
        return ANGLE_ANY;
    else
        //since the front point has an unknown incoming angle, use the next point
        return ANGLE_OPPOSITE(operator[](1).angle);
}

double Optimizer_Path::exit_angle()
{
    return back().angle; // (size() < 2) is tested and handled by operator[]
}



Optimizer_Point Optimizer_Path::front()
{
    return operator[](0);
}


Optimizer_Point Optimizer_Path::back()
{
    return operator[](size() - 1);
}


//in-order point lookup function
//transparently handles inverted paths, and angle recalculation
Optimizer_Point Optimizer_Path::operator[](size_t n)
{
    Optimizer_Point p;

    //lookup the raw point
    if(INVERTED)
        p = points->at(a - n);
    else
        p = points->at(a + n);


    //correct the points' angle field
    if(size() < 2)
    {
        //if there aren't enough points, then the angle is irrelevant
        p.angle = ANGLE_ANY;
    }
    else
    {
        //if we're at the beginning of a path, diregard existing incoming
        //angle information, since paths are considered to be isolated elements
        if(n == 0)
            p.angle = ANGLE_ANY;

        if(INVERTED)
        {
            //lookup the previous point (from the perspective of the INVERTED path)
            Optimizer_Point previous = points->at(a - (n - 1));
            p.angle = ANGLE(previous.point, p.point);
        }
        //else, don't the angles are fine, they've already been computed in the
        //forward direction
    }

    return p;
}



} // namespace lzr
