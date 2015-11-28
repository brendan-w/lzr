
#include "optimizer.h"

namespace lzr {



static inline double deg_to_rad(double deg)
{
    return deg * PI / 180.0;
}



/*
 * Wrapper for the user-facing optimizer,
 * also stores settings between runs
 */

Optimizer::Optimizer()
{
    ctx = new Optimizer_Context();
}


Optimizer::~Optimizer()
{
    delete ctx;
}


//main optimization function
int Optimizer::run(Frame& frame)
{
    return ctx->run(frame, deg_to_rad(path_split_angle),
                           reorder_paths,
                           anchor_points_lit,
                           anchor_points_blanked);
}




/*
 * Internal optimizer context
 * Main optimization function
 */


int Optimizer_Context::run(Frame& frame, double split_angle, bool reorder, size_t lit_anchors, size_t blanked_anchors)
{
    //save the frame to our working buffer
    points.resize(frame.size());
    for(size_t i = 0; i < frame.size(); i++)
    {
        points[i].point = frame[i];
    }

    find_paths(split_angle);

    if(reorder)
        reorder_paths(); //sorts the path buffer

    // compile_paths(frame, lit_anchors, blanked_anchors);

    return 0;
}



/*
 * Paths
 */

size_t Optimizer_Path::size()
{
    if(a < b)
        return b - a + 1;
    else
        return a - b + 1;
}

void Optimizer_Path::invert()
{
    size_t temp = a;
    a = b;
    b = temp;
}


} // namespace lzr
