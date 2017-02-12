
#include "optimizer.hpp"
#include "interpolator.hpp"

namespace lzr {



/*
 * Wrapper for the user-facing optimizer,
 * also stores settings between runs
 */

Optimizer::Optimizer()
{
    //assume the laser to be in a default position
    internal = new OptimizerInternals(Point());
}


Optimizer::Optimizer(Point current_laser_position)
{
    internal = new OptimizerInternals(current_laser_position);
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
 */


OptimizerInternals::OptimizerInternals(Point current_laser_position)
{
    last_known_point = current_laser_position;
    last_known_point.angle = ANGLE_ANY;
    num_last_known_anchors = 0;
}


//Main optimization function
int OptimizerInternals::run(Optimizer* settings, Frame& frame)
{
    //save the frame to our working buffer
    points.resize(frame.size());
    for(size_t i = 0; i < frame.size(); i++)
    {
        points[i] = frame[i];
    }

    find_paths(settings);

    if(settings->reorder_paths)
    {
        reorder_paths(settings); //sorts the path buffer
    }

    compile_paths(settings, frame);

    interpolate(frame, settings->interp_distance, settings->interp_func);

    return 0;
}



/*
 * Optimizer Points
 */

const Optimizer_Point& Optimizer_Point::operator=(const Point& rhs)
{
    if(this == &rhs)
        return *this;

    x = rhs.x;
    y = rhs.y;
    r = rhs.r;
    g = rhs.g;
    b = rhs.b;
    i = rhs.i;

    return *this;
}

Point Optimizer_Point::to_point()
{
    Point p;
    p.x = x;
    p.y = y;
    p.r = r;
    p.g = g;
    p.b = b;
    p.i = i;
    return p;
}


} // namespace lzr
