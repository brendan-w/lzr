
#include <stdio.h>
#include "optimizer.h"
#include "interpolator.h"

namespace lzr {



//lookups for the number of existing anchor points in the given path
size_t Optimizer_Internals::num_beginning_anchors(Optimizer_Path path)
{
    size_t n = 1;

    Point first_point = points[path.a].point;

    //loop forwards
    for(int i = 0; i < ((int)path.size()); i++)
    {
        if(first_point == points[path[i]].point)
            n++;
        else
            break;
    }

    return n;
}

/*
size_t Optimizer_Internals::num_ending_anchors(Optimizer_Path path)
{
    size_t n = 1;

    Point last_point = points[path.b].point;

    //loop backwards
    for(int i = ((int)path.size()) - 1; i >= 0; i--)
    {
        if(last_point == points[path[i]].point)
            n++;
        else
            break;
    }

    return n;
}
*/

//counts the number of anchor points at the end of the frame
size_t Optimizer_Internals::num_ending_anchors(Frame& frame)
{
    size_t n = 1;

    Point last_point = frame.back();

    //loop backwards
    for(int i = ((int)frame.size()) - 1; i >= 0; i--)
    {
        if(last_point == frame[i])
            n++;
        else
            break;
    }

    return n;
}



/*
 * This function will look at the end of the current frame, as well as the
 * start of the given path, and will insert any additional needed LIT
 * anchor points.
 */
void Optimizer_Internals::add_path_to_frame(Optimizer* settings,
                                            Frame& frame,
                                            Optimizer_Path path)
{
    int anchors = settings->anchor_points_lit;
    anchors -= num_beginning_anchors(path); //the number of leading anchors that are already present

    if(frame.back().is_lit())
    {
        //if the previous point was lit (aka, we're continuing a solid line)
        anchors -= num_ending_anchors(frame); //the number of anchors already at the end of the frame
    }

    //write any additional lit anchor points
    for(int i = 0; i < anchors; i++)
    {
        frame.add(points[path.a].point);
    }

    //write the path
    for(size_t i = 0; i < path.size(); i++)
    {
        frame.add(points[path[i]].point);
    }
}



//generates a blanking jump between two Point's
void Optimizer_Internals::blank_between(Optimizer* settings,
                                        Frame& frame,
                                        Point a,
                                        Point b)
{
    a.blank();
    b.blank();

    for(size_t i = 0; i < settings->anchor_points_blanked; i++)
        frame.add(a);

    //interpolate from A to B
    interp_line(frame, settings->blank_max_distance, settings->blank_func, a, b);

    for(size_t i = 0; i < settings->anchor_points_blanked; i++)
        frame.add(b);
}


void Optimizer_Internals::compile_paths(Optimizer* settings, Frame& frame)
{
    frame.clear();

    for(Optimizer_Path path : paths)
    {
        Point a = last_known_point.point; //last_known_point is gauranteed to be lit
        Point b = points[ path.a ].point; //first point on the current path

        if( ! a.same_position_as(b) )
        {
            //if the last_known_point is different than this frames
            //starting point, then an introductory blanking jump
            //is neccessary.
            blank_between(settings, frame, a, b);
        }

        //load the drawn points into the output buffer
        add_path_to_frame(settings, frame, path);

        //walk the laser
        last_known_point = points[path.b];
    }
}



} // namespace lzr