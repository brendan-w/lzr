

#include "optimizer.h"

namespace lzr {



//the cost function for a blank jump between points A and B
double Optimizer_Context::cost(Optimizer_Point a, Optimizer_Point b)
{
    return a.point.sq_distance_to(b.point);
}

void Optimizer_Context::swap_paths(Optimizer_Path* path_a, Optimizer_Path* path_b)
{
    if(path_a != path_b)
    {
        Optimizer_Path temp = *path_a;
        *path_a = *path_b;
        *path_b = temp;
    }
}

//scan for the best path to enter next
path_choice Optimizer_Context::find_next(size_t start, Optimizer_Point laser)
{
    //running vars
    path_choice pc;        //the best path
    double c;              //temp var for computing cost
    double min_cost = 0.0; //optimize for least cost


    //initial check
    //lookup the point at the head of the first path to be checked
    min_cost = c = cost(laser, points[paths[start].a]);
    pc.path = start;
    pc.invert = false;
    
    for(size_t i = start + 1; i < paths.size(); i++)
    {
        Optimizer_Path path = paths[i];

        //test the front point (A)
        c = cost(laser, points[path.a]);
        if(c < min_cost)
        {
            min_cost = c;
            pc.path = i;
            pc.invert = false;
        }

        //test the back point (B)
        c = cost(laser, points[path.b]);
        if(c < min_cost)
        {
            min_cost = c;
            pc.path = i;
            pc.invert = true;
        }
    }

    return pc;
}

void Optimizer_Context::reorder_paths()
{
    Optimizer_Point laser = last_known_point;

    for(size_t i = 0; i < paths.size(); i++)
    {
        Optimizer_Path* path = &paths[i];

        path_choice next = find_next(i, laser);

        swap_paths(path, &paths[next.path]);

        if(next.invert)
            path->invert();

        //update the laser's current location
        laser = points[path->b];
    }
}


} // namespace lzr
