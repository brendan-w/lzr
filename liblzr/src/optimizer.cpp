
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


} // namespace lzr
