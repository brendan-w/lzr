
#include <cmath>
#include <liblzr.hpp>

namespace lzr {


/*
 * Interpolation functions
 * Domain: [0.0, 1.0]
 * Range:  [0.0, 1.0]
 */

static const double b = 0.0; //start value
static const double c = 1.0; //change in value
static const double d = 1.0; //duration

double linear(double t)
{
    return t;
}

double quad(double t)
{
    t /= d/2;
    if(t < 1) return c/2*t*t + b;
    t--;
    return -c/2 * (t*(t-2) - 1) + b;
}

double quart(double t)
{
    t /= d/2;
    if(t < 1) return c/2*t*t*t*t + b;
    t -= 2;
    return -c/2 * (t*t*t*t - 2) + b;
}




//generates extra points on a line
//only adds the interior points, not the endpoints
int interp_line(Frame& working, double max_distance, interpolation_func func, Point start, Point end)
{
    //find out if the endpoints of the line are far enough apart to require interpolation
    double sq_dist     = start.sq_distance_to(end);
    double sq_max_dist = max_distance * max_distance;

    if(sq_dist > sq_max_dist)
    {
        //time to interpolate

        //number of intersticial points to generate
        //(square-root things back to actual values)
        size_t n = (size_t) (std::sqrt(sq_dist) / max_distance); //integer division provides flooring

        n += 2; //include the two endpoints, which already exist (clarity, nothing more)

        Point prev = start;

        //loop through the intersticial points
        for(size_t i = 1; i < n; i++)
        {
            //get the normalized position of the new point
            double t = (double) i / n;

            //apply the user's interpolation function
            t = func(t);

            //generate the new point
            Point p = start.lerp_to(end, t);

            //prevent multiple points at the same location
            if((p != start) && (p != end))
            {
                working.add(p);
                prev = p;
            }
        }
    }

    return LZR_SUCCESS;
}


int interpolate(Frame& frame, double max_distance, interpolation_func func)
{
    //prevent stupidity
    if(max_distance == 0.0)
        return LZR_ERROR_INVALID_ARG;

    //a working buffer for us to build in
    Frame working;

    bool in_path = false; //whether or not the previous point was a lit point
    Point prev;

    for(size_t i = 0; i < frame.size(); i++)
    {
        Point p = frame[i];

        if(p.is_blanked())
        {
            in_path = false;
        }
        else if(!in_path)
        {
            in_path = true;
        }
        else
        {
            interp_line(working, max_distance, func, prev, p);
        }

        prev = p;
        working.add(p);
    }

    //write the finished points back to the user's frame
    frame = working;

    return LZR_SUCCESS;
}


} // namespace lzr
