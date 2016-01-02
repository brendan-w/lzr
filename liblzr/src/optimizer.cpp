
#include "optimizer.h"

namespace lzr {



/*
 * Wrapper for the user-facing optimizer,
 * also stores settings between runs
 */

Optimizer::Optimizer()
{
    internal = new Optimizer_Internals();
}


Optimizer::~Optimizer()
{
    delete internal;
}


//main optimization function
int Optimizer::run(Frame& frame)
{
    return internal->run(this, frame);
}




/*
 * Internal optimizer context
 * Main optimization function
 */


int Optimizer_Internals::run(Optimizer* settings, Frame& frame)
{
    //save the frame to our working buffer
    points.resize(frame.size());
    for(size_t i = 0; i < frame.size(); i++)
    {
        points[i].point = frame[i];
    }

    find_paths(settings);

    if(settings->reorder_paths)
        reorder_paths(settings); //sorts the path buffer

    compile_paths(settings, frame);

    return 0;
}



/*
 * Paths
 */

size_t Optimizer_Path::size()
{
    if(a < b)
        return b - a + 1; //non-inverted
    else
        return a - b + 1; //inverted
}

void Optimizer_Path::invert()
{
    size_t temp = a;
    a = b;
    b = temp;
}

double Optimizer_Path::entrance_angle(const std::vector<Optimizer_Point> & points)
{
    //if there aren't enough points, then the angle is irrelevant
    if(size() < 2)
    {
        return ANGLE_ANY;
    }
    else
    {
        if(a < b) //non-inverted
            return points[a + 1].angle;
        else
            return ANGLE_OPPOSITE(points[b].angle);
    }
}

double Optimizer_Path::exit_angle(const std::vector<Optimizer_Point> & points)
{
    //if there aren't enough points, then the angle is irrelevant
    if(size() < 2)
    {
        return ANGLE_ANY;
    }
    else
    {
        if(a < b)
            return points[b].angle;
        else
            return ANGLE_OPPOSITE(points[a + 1].angle);
    }
}



//in-order point index lookup function
//transparently handles inverted paths
size_t Optimizer_Path::operator[](size_t n)
{
    //test if the path is inverted
    if(a < b)
        return a + n; //non-inverted
    else
        return a - n; //inverted
}




} // namespace lzr
