
/*
    This file attempts to minimize the amount of time spent blanking and
    changing directions. It does this by rearranging and/or inverting paths
    in the "paths" buffer. This is essentially a travelling salesman problem
    with vector paths. For efficiency, this uses the sub-optimal
    next-cheapest-option algorithm (for now).
*/



#include <algorithm> //swap()
#include "optimizer.h"

namespace lzr {


/*

    The cost function

    Looks at two things:
        - Distance to next path
        - Angle deviation

    The angle deviation clause is designed to help make choices like this:

    A       B               A       B

    |      .|               |       |
    |     . |               |       |
    |    .  |               |       |
    v   .   v      VS.      v       ^
    |  .    |               |       |
    | .     |               |       |
    |.      |               |.......|


    The total angular deflection (hassle) for a particular jump is given by:

    abs(blank_jump_angle - path_A_exit_angle) + abs(path_B_entrance_angle - blank_jump)
*/

double Optimizer_Internals::cost(Optimizer_Point laser, Optimizer_Path path)
{
    double blank_angle = ANGLE(laser.point, path.front(points).point); //from the end of A to the start of B
    double total_angular_deflection = 0.0;
    total_angular_deflection += std::abs(blank_angle - laser.angle); //going IN to the blanking jump
    total_angular_deflection += std::abs(path.entrance_angle() - blank_angle); //coming OUT of the blanking jump

    return laser.point.sq_distance_to(path.front(points).point);
}


//scan for the best path to enter next
void Optimizer_Internals::find_next_and_swap(size_t current_path, Optimizer_Point laser)
{
    //running vars
    size_t best_path;      //index of the path with the best (lowest) cost
    bool invert;           //whether or not the best_path should be inverted
    double c;              //temp var for computing cost
    double min_cost = 0.0; //optimize for least cost


    //helper macro for recording stats
    #define STORE_BEST_PATH(inverted) \
    {                                 \
        min_cost = c;                 \
        best_path = i;                \
        invert = inverted;            \
    }                                 \


    //initial check
    //lookup the point at the head of the first path to be checked
    min_cost = c = cost(laser, paths[current_path]);
    best_path = current_path;
    invert = false;

    //look for the next best path
    for(size_t i = current_path; i < paths.size(); i++)
    {
        Optimizer_Path path = paths[i];

        //test the path as-is
        c = cost(laser, path);
        if(c < min_cost) STORE_BEST_PATH(false);

        //test the inverted version of the same path
        path.invert();
        c = cost(laser, path);
        if(c < min_cost) STORE_BEST_PATH(true);
    }

    //we're all done, apply the change
    std::swap(paths[current_path], paths[best_path]);

    if(invert)
        paths[current_path].invert();


    #undef STORE_BEST_PATH
}


void Optimizer_Internals::reorder_paths(Optimizer* settings)
{
    (void) settings;

    Optimizer_Point laser = last_known_point;

    for(size_t i = 0; i < paths.size(); i++)
    {
        //find and process the best path at this index
        find_next_and_swap(i, laser);

        //update the laser's current location
        laser = paths[i].back(points);
    }
}


} // namespace lzr
