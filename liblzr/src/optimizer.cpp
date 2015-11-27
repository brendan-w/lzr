
#include "optimizer.h"

namespace lzr {


static inline double deg_to_rad(double deg)
{
    return deg * PI / 180.0;
}

/*
 * user-facing optimizer
 */

Optimizer::Optimizer()
{
    ctx = new Optimizer_Context();
}


Optimizer::~Optimizer()
{
    delete ctx;
}


int Optimizer::run(Frame& frame)
{
    ctx->load_points(frame);
    ctx->find_paths(deg_to_rad(path_split_angle));
    // rearrange_paths(opt);       //sorts the path buffer
    // compile_paths(opt, frame); //updates the point buffer and generates blanking jumps
    return 0;
}




/*
 * Internal optimizer context
 */


void Optimizer_Context::load_points(Frame& frame)
{
    //load the new points into the working buffer (overwrite any old data)
    points.resize(frame.size());

    for(size_t i = 0; i < frame.size(); i++)
    {
        points[i].point = frame[i];
    }
}

//other context functions are defined in their respective files


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



} // namespace lzr
