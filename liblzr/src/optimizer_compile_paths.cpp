

#include "optimizer.h"
#include "interpolator.h"

namespace lzr {




//lookups for the number of existing anchor points
size_t Optimizer_Internals::num_ending_anchors(Optimizer_Path path)
{
    size_t n = 1;

    Point last_point = points[path.b].point;

    for(int i = ((int)path.size()) - 1; i >= 0; i--)
    {
        if(last_point == points[path[i]].point)
            n++;
        else
            break;
    }

    return n;
}


void Optimizer_Internals::add_path_to_frame(Optimizer* settings,
                                            Frame& frame,
                                            Optimizer_Path path,
                                            bool skip_first_point)
{
    size_t skip = (skip_first_point ? 1 : 0);

    for(size_t i = skip; i < path.size(); i++)
    {
        frame.add(points[path[i]].point);
    }

    //make sure that the requested number of lit anchor points are present at the end of the path
    int anchors = settings->anchor_points_lit - num_ending_anchors(path);

    //add any extra needed anchors
    for(int i = 0; i < anchors; i++)
    {
        frame.add(points[path.b].point);
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
        //number of points to skip at the start of the path
        bool skip_first_point = false;

        Point a = last_known_point.point; //last_known_point is gauranteed to be lit
        Point b = points[ path.a ].point; //first point on the current path

        if( a.same_position_as(b) )
        {
            //if the start of this path is the same as the last_known_point,
            //then skip the beginning point
            skip_first_point = true;
        }
        else
        {
            //if the last_known_point is different than this frames
            //starting point, then an introductory blanking jump
            //is neccessary.
            blank_between(settings, frame, a, b);
        }

        //load the drawn points into the output buffer
        add_path_to_frame(settings, frame, path, skip_first_point);

        //walk the laser
        last_known_point = points[path.b];
    }
}



} // namespace lzr
