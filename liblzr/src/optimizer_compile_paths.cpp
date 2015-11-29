

#include "optimizer.h"

namespace lzr {




void Optimizer_Context::add_path_to_frame(Frame& frame, Optimizer_Path path, bool skip_first_point)
{
    size_t skip = (skip_first_point ? 1 : 0);

    //test if the path is inverted
    if(path.a < path.b)
    {
        //ascending from the path's A point
        for(size_t i = skip; i < path.size(); i++)
        {
            frame.add(points[path.a + i].point);
        }
    }
    else
    {
        //descending from the path's B point
        for(size_t i = skip; i < path.size(); i++)
        {
            frame.add(points[path.a - i].point);
        }
    }
}



//generates a blanking jump between two Point's
void Optimizer_Context::blank_between(Frame& frame, Point a, Point b, size_t anchors)
{
    a.blank();
    b.blank();

    for(size_t i = 0; i < anchors; i++)
        frame.add(a);

    for(size_t i = 0; i < anchors; i++)
        frame.add(b);
}


void Optimizer_Context::compile_paths(Frame& frame, size_t lit, size_t blanked)
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
            blank_between(frame, a, b, blanked);
        }

        //load the drawn points into the output buffer
        add_path_to_frame(frame, path, skip_first_point);

        //walk the laser
        last_known_point = points[path.b];
    }
}



} // namespace lzr
