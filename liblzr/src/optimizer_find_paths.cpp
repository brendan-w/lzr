

#include "optimizer.h"

namespace lzr {



/*
    The angle formed between three points. Because of the way point
    angles are stored, only points B and C are neccessary.
    (see fill_angle() for details)

              C .
               /
              /
    A      B /
    ._______.
*/
#define ANGLE_FORMED(b, c) (std::abs(b.angle - c.angle))



//computes the angles from one point to the next
void Optimizer_Context::fill_angle()
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

    points[0].angle = ANGLE(last_known_point.point,
                                 points[0].point);

    for(size_t i = 1; i < points.size(); i++)
    {
        points[i].angle = ANGLE(points[i - 1].point,
                                points[i].point);
    }
}



void Optimizer_Context::path_split(double split_angle)
{
    paths.clear(); //dump whatever we had before

    bool in_path = false; //whether the loop is inside an unterminated path
    Optimizer_Path path;  //path currently being built

    for(size_t i = 0; i < points.size(); i++)
    {
        Optimizer_Point p = points[i];

        if(p.point.is_blanked())
        {
            if(in_path)
            {
                //encountered first blanked point when IN a path
                //close the open path
                path.b = i - 1;
                paths.push_back(path);
                in_path = false;
            }
            // else, do nothing, discard blanked points
        }
        else //a lit point
        {
            if(in_path)
            {
                //test the angle this point makes with previous/next points
                if( (i+1 < paths.size()) && !points[i+1].point.is_blanked() ) //is the next point valid to check against
                {
                    Optimizer_Point next = points[i + 1];

                    //if it creates too much of an angle
                    if(ANGLE_FORMED(p, next) > split_angle)
                    {
                        //close the current path
                        path.b = i;
                        paths.push_back(path);
                        //open a new one
                        path.a = i;
                    }
                }
            }
            else
            {
                //encountered first lit point when not in a path
                //open a new path
                path.a = i; //store the index of the first point in the path
                in_path = true;
            }
        }
    }

    //if a path is still open, close it (frame ends in a lit point)
    if(in_path)
    {
        path.b = points.size() - 1;
        paths.push_back(path);
        in_path = false;
    }
}



void Optimizer_Context::fill_cycle(double split_angle)
{
    for(Optimizer_Path& path : paths)
    {
        Optimizer_Point a = points[path.a];
        Optimizer_Point b = points[path.b];

        //if they're in the same position, and there are at least 3 points
        if(a.point.same_position_as(b.point) && path.size() >= 3)
        {
            //fetch the point one forward of the joint
            Optimizer_Point next = points[path.a + 1];

            //if it DOESN'T creates too much of an angle, then it's a cycle
            path.cycle = (ANGLE_FORMED(b, next) <= split_angle);
        }
    }
}


/*
    Splits a point buffer into individual path segments.
    Loads the result into the given path buffer.
*/
void Optimizer_Context::find_paths(double split_angle)
{
    fill_angle();
    path_split(split_angle);
    fill_cycle(split_angle);
}


} // namespace lzr
