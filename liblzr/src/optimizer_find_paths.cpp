
/*
    This file is largely for setting things up, and computing extra data about
    the points.

    It first computes the angles from each point to the next. This gets stored
    in each Optimizer_Point in the point buffer. This is the ONLY TIME that the
    contents of the point buffer should be modified.

    It then splits the points into "paths" based on blanking changes and hard
    corners. A path is described simply by the index of its starting and ending
    points. All paths live in the "paths" buffer on the optimizer context.
*/





#include "optimizer.hpp"

namespace lzr {



//computes the angles from one point to the next
void OptimizerInternals::fill_angle()
{
    /*

        When computing the angles from one point to the next,
        the angle stored on each point is the INCOMING angle for that point.
        For the outgoing angle, use the value from the next point.
        The first point's angle is computed based on the last_known_point.

                                   angle
                |---->|           |---->|

                *    [P     P     P     P     P     P     ...]

                |                       |
        last_known_point           stored here

    */

    points[0].angle = POINT_ANGLE(last_known_point,
                                  points[0]);

    for(size_t i = 1; i < points.size(); i++)
    {
        if(points[i].is_lit())
            points[i].angle = POINT_ANGLE(points[i - 1],
                                          points[i]);
        //else, skip blanked points, they get recalculated anyway
    }
}



void OptimizerInternals::path_split(double split_angle)
{
    paths.clear(); //dump whatever we had before

    bool in_path = false; //whether the loop is inside an unterminated path
    size_t a; //index for the starting point of a path


    for(size_t i = 0; i < points.size(); i++)
    {
        Optimizer_Point p = points[i];

        if(p.is_blanked())
        {
            if(in_path)
            {
                //encountered first blanked point when IN a path
                //close the open path
                paths.push_back(Optimizer_Path(a, i - 1, points));
                in_path = false;
            }
            // else, do nothing, discard blanked points
        }
        else //a lit point
        {
            if(in_path)
            {
                //test the angle this point makes with previous/next points
                if( (i+1 < paths.size()) && points[i+1].is_lit() ) //is the next point valid to check against
                {
                    Optimizer_Point next = points[i + 1];

                    //if it creates too much of an angle
                    if(POINT_DEFLECTION(p, next) >= split_angle)
                    {
                        //close the current path
                        paths.push_back(Optimizer_Path(a, i, points));
                        //open a new one
                        a = i;
                    }
                }
            }
            else
            {
                //encountered first lit point when not in a path
                //open a new path
                a = i; //store the index of the first point in the path
                in_path = true;
            }
        }
    }

    //if a path is still open, close it (frame ends in a lit point)
    if(in_path)
    {
        paths.push_back(Optimizer_Path(a, points.size() - 1, points));
        in_path = false;
    }
}



void OptimizerInternals::fill_cycle(double split_angle)
{
    for(Optimizer_Path& path : paths)
    {
        Optimizer_Point a = path.front(points);
        Optimizer_Point b = path.back(points);

        //if they're in the same position, and there are at least 3 points
        if(a.same_position_as(b) && path.size() >= 3)
        {
            //fetch the point one forward of the joint
            Optimizer_Point next = path.at(1, points);

            //if it DOESN'T creates too much of an angle, then it's a cycle
            path.cycle = (POINT_DEFLECTION(b, next) < split_angle);
        }
    }
}


/*
    Splits a point buffer into individual path segments.
    Loads the result into the given path buffer.
*/
void OptimizerInternals::find_paths(Optimizer* settings)
{
    double split_angle = DEG_TO_RAD(settings->path_split_angle);

    fill_angle();
    path_split(split_angle);
    fill_cycle(split_angle);
}


} // namespace lzr
