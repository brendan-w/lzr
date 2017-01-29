
#include "optimizer.hpp"

namespace lzr {



//handy test for whether the path has been inverted
#define INVERTED (b < a)



Optimizer_Path::Optimizer_Path(size_t _a, size_t _b, const std::vector<Optimizer_Point> & points)
{
    a = _a;
    b = _b;

    //note the entrance and exit angles
    if(size() < 2)
    {
        entrance = ANGLE_ANY;
        exit = ANGLE_ANY;
    }
    else
    {
        entrance = points[a+1].angle;
        exit = points[b].angle;
    }
}


size_t Optimizer_Path::size() const
{
    if(INVERTED)
        return a - b + 1;
    else
        return b - a + 1;
}


//swaps A and B
void Optimizer_Path::invert()
{
    //swap the point indicies
    size_t temp = a;
    a = b;
    b = temp;
}


double Optimizer_Path::entrance_angle() const
{
    if(INVERTED)
        return ANGLE_OPPOSITE(exit);
    else
        return entrance;
}

double Optimizer_Path::exit_angle() const
{
    if(INVERTED)
        return ANGLE_OPPOSITE(entrance);
    else
        return exit;
}


//lookups for the number of existing anchor points in the given path
size_t Optimizer_Path::front_anchors(const std::vector<Optimizer_Point> & points) const
{
    size_t n = 1;

    Optimizer_Point first_point = front(points);

    //loop forwards until we find a point that differs
    for(size_t i = 1; i < size(); i++)
    {
        if(first_point == at(i, points))
            n++;
        else
            break;
    }

    return n;
}


//currently not used, but implemented just in case
size_t Optimizer_Path::back_anchors(const std::vector<Optimizer_Point> & points) const
{
    size_t n = 1;

    Optimizer_Point last_point = back(points);

    //loop backwards, cast to int to avoid underflow errors
    for(int i = ((int)size()) - 2; i >= 0; i--)
    {
        if(last_point == at(i, points))
            n++;
        else
            break;
    }

    return n;
}


Optimizer_Point Optimizer_Path::front(const std::vector<Optimizer_Point> & points) const
{
    return at(0, points); //TODO throw errors for path of size == 0
}


Optimizer_Point Optimizer_Path::back(const std::vector<Optimizer_Point> & points) const
{
    return at(size() - 1, points); //TODO throw errors for path of size == 0
}


//in-order point lookup function
//transparently handles inverted paths, and angle recalculation
//TODO: handle invalid point indicies
Optimizer_Point Optimizer_Path::at(size_t n, const std::vector<Optimizer_Point> & points) const
{
    Optimizer_Point p;

    //lookup the raw point
    if(INVERTED)
        p = points[a - n];
    else
        p = points[a + n];


    //correct the points' angle field
    if(size() < 2)
    {
        //if there aren't enough points, then the angle is irrelevant
        p.angle = ANGLE_ANY;
    }
    else if(n == 0)
    {
        //if we're at the beginning of a path, diregard existing incoming
        //angle information, since paths are considered to be isolated elements
        p.angle = ANGLE_ANY;
    }
    else
    {
        if(INVERTED)
        {
            //lookup the previous point (from the perspective of the INVERTED path)
            Optimizer_Point previous = points[a - (n - 1)];
            p.angle = ANGLE_OPPOSITE(previous.angle);
        }
        //else, the angles are fine, since they've already been computed
        //in the forward direction
    }

    return p;
}



} // namespace lzr
