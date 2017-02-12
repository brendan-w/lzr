
/*
    This file attempts to minimize the amount of time spent blanking and
    changing directions. It does this by rearranging and/or inverting paths
    in the "paths" buffer. This is essentially a travelling salesman problem
    with vector paths. For efficiency, this uses the sub-optimal
    next-cheapest-option algorithm (for now).
*/



#include <algorithm> //swap()
#include "optimizer.hpp"

namespace lzr {


#define MAX_DEFLECTION (2 * PI)
#define MAX_SQ_DISTANCE 2.0

//special values for weighting each metric
#define SQ_DISTANCE_COEFFICIENT 0.9
#define DEFLECTION_COEFFICIENT 0.1




/*

    The cost function(s)

    Looks at two things:
        - Distance to next path
        - Angle deviation

    The angle deviation clause is designed to help make choices like this:

    A       B               A       B'

    |      .|               |       |
    |     . |               |       |
    |    .  |               |       |
    v   .   v      VS.      v       ^
    |  .    |               |       |
    | .     |               |       |
    |.      |               |.......|


    The total angular deflection (hassle) for a particular jump is given by:

    abs(blank_jump_angle - path_A_exit_angle) + abs(path_B_entrance_angle - blank_jump)

    In cases where no blanking jump is required, choices like this
    are judged only by angular deflection.


        |
        |
        v
        |
        |
        *
       /|\
      / | \
     /  |  \
    A   B   C
*/

//the deflection gets calculated differently if there's a blanking jump,
//or if any angle returns ANGLE_ANY
double OptimizerInternals::angular_deflection(const Optimizer_Point& laser, const Optimizer_Path& path)
{
    double deflection = 0.0;

    //grab the current angle of travel for the laser, and the entrance angle
    //for the potential path
    double laser_angle = laser.angle;
    double path_angle  = path.entrance_angle();


    //if distance is basically zero
    if(laser.same_position_as(path.front(points)))
    {
        //if either angle has no preference, then ignore 
        if((laser_angle != ANGLE_ANY) &&
           (path_angle != ANGLE_ANY))
        {
            deflection = ANGLE_DEFLECTION(laser_angle, path_angle);
        }
    }
    else
    {
        //a blank jump is needed
        double blank_angle = POINT_ANGLE(laser, path.front(points)); //from the end of A to the start of B

        //if any of these angles returned ANGLE_ANY, then they shouldn't contribute
        //any angular deflection. There's no need to worry about them.
        laser_angle = (laser_angle == ANGLE_ANY) ? blank_angle : laser_angle;
        path_angle  = (path_angle  == ANGLE_ANY) ? blank_angle : path_angle;

        //how much of a hassle is this direction change going to be?
        deflection += ANGLE_DEFLECTION(laser_angle, blank_angle); //going IN to the blanking jump
        deflection += ANGLE_DEFLECTION(blank_angle, path_angle); //coming OUT of the blanking jump
    }

    return deflection;
}


double OptimizerInternals::cost(const Optimizer_Point laser, const Optimizer_Path path)
{
    //metrics
    double sq_distance = laser.sq_distance_to(path.front(points)); // [0, 2]
    double deflection = angular_deflection(laser, path);           // [0, 2Pi)

    //normalize the costs
    sq_distance /= MAX_SQ_DISTANCE;
    deflection /= MAX_DEFLECTION;

    //use addition to prevent zeros from overwhelming the other metrics
    return (sq_distance * SQ_DISTANCE_COEFFICIENT) +
           (deflection * DEFLECTION_COEFFICIENT);
}


//scan for the best path to enter next
void OptimizerInternals::find_next_and_swap(const size_t current_path, const Optimizer_Point laser)
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


void OptimizerInternals::reorder_paths(Optimizer* settings)
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
