

#include "optimizer.h"

namespace lzr {



//the cost function for a blank jump between points A and B
double Optimizer_Internals::cost(Optimizer_Point a, Optimizer_Point b)
{
    return a.point.sq_distance_to(b.point);
}


void Optimizer_Internals::swap_paths(size_t a, size_t b)
{
    if(a != b)
    {
        Optimizer_Path temp = paths[a];
        paths[a] = paths[b];
        paths[b] = temp;
    }
}


//scan for the best path to enter next
void Optimizer_Internals::find_next_and_swap(size_t current_path, Optimizer_Point laser)
{
    //running vars
    size_t best_path;      //index of the path with the best (lowest) cost
    bool invert;           //whether or not the best_path should be inverted
    double c;              //temp var for computing cost
    double min_cost = 0.0; //optimize for least cost


    //initial check
    //lookup the point at the head of the first path to be checked
    min_cost = c = cost(laser, points[paths[current_path].a]);
    best_path = current_path;
    invert = false;


    for(size_t i = current_path; i < paths.size(); i++)
    {
        Optimizer_Path path = paths[i];

        //test the front point (A)
        c = cost(laser, points[path.a]);
        if(c < min_cost)
        {
            min_cost = c;
            best_path = i;
            invert = false;
        }

        //test the back point (B)
        c = cost(laser, points[path.b]);
        if(c < min_cost)
        {
            min_cost = c;
            best_path = i;
            invert = true;
        }
    }

    //we're all done, apply the change
    swap_paths(current_path, best_path);

    if(invert)
        paths[current_path].invert();
}


void Optimizer_Internals::reorder_paths(Optimizer* settings)
{
    Optimizer_Point laser = last_known_point;

    for(size_t i = 0; i < paths.size(); i++)
    {
        //find and process the best path at this index
        find_next_and_swap(i, laser);

        //update the laser's current location
        laser = points[ paths[i].b ];
    }
}


} // namespace lzr
