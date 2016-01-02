
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

//in-order point index lookup function
//transparently handles inverted paths
size_t Optimizer_Path::operator[](size_t n)
{
    //test if the path is inverted
    if(a < b)
        return a + n;
    else
        return a - n;
}




} // namespace lzr
