

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
path_descriptor Optimizer_Context::find_next(size_t start, Optimizer_Point laser)
{
    //running vars
    path_descriptor pd;        //the best path
    Optimizer_Point* possible; //temp var for testing a path
    double c;                  //temp var for computing cost
    double min_cost = 0.0;     //optimize for least cost


    //initial check
    possible = &points[paths[start].a]; //lookup the point at the head of the first path to be checked
    min_cost = c = cost(laser, *possible);
    pd.path = &paths[start];
    pd.invert = false;
    
    for(size_t i = start; i < paths.size(); i++)
    {
        Optimizer_Path* current = &paths[i];

        //test the front point (A)
        possible = &points[paths[start].a];
        c = cost(laser, *possible);
        if(c < min_cost)
        {
            min_cost = c;
            pd.path = current;
            pd.invert = false;
        }

        //test the back point (B)
        possible = &points[paths[start].b];
        c = cost(laser, *possible);
        if(c < min_cost)
        {
            min_cost = c;
            pd.path = current;
            pd.invert = true;
        }
    }

    return pd;
}

void Optimizer_Context::reorder_paths()
{
    Optimizer_Point laser = last_known_point;

    for(size_t i = 0; i < paths.size(); i++)
    {
        Optimizer_Path* current = &paths[i];

        path_descriptor next = find_next(i, laser);
        swap_paths(current, next.path);

        if(next.invert)
            current->invert();

        //update the laser's current location
        laser = points[current->b];
    }
}


} // namespace lzr
