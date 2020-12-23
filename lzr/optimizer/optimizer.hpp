#pragma once

#include "lzr/core/core.hpp"

/******************************************************************************/
/*  LZR Optimizer                                                             */
/*  Converts a stream of raw frames into something friendly for the scanners  */
/*  Handles:                                                                  */
/*    - interpolation (for both blanked and lit lines)                        */
/*    - anchor points                                                         */
/*    - path scan order                                                       */
/*    - inter-frame blanking                                                  */
/******************************************************************************/

namespace lzr {

//interpolation point density (points from one side of the frame to the other)
static constexpr float INTERP_DEFAULT = ((Point::POSITION_MAX - Point::POSITION_MIN) / 100.0);
static constexpr float BLANK_INTERP_DEFAULT = ((Point::POSITION_MAX - Point::POSITION_MIN) / 5.0);

//fwrd decl
class OptimizerInternals;

class LIBLZR_EXPORT Optimizer
{
public:
    Optimizer();
    Optimizer(Point current_laser_position);
    ~Optimizer();

    //main optimizer function
    int run(Frame& frame);

    // ----- settings -----

    float path_split_angle = 45; //minimum angle (degrees) at which to consider lines to be seperate paths
    bool  reorder_paths = true;  //allow the optimizer to the change the order in which points are scanned

    //anchor points
    size_t anchor_points_lit = 1;      //minimum number of lit points to place at the start & end of line segments
    size_t anchor_points_blanked  = 2; //minimum number of blanked points to place at the start & end of a line segment

    //interpolation
    float interp_distance = INTERP_DEFAULT;  //max distance for interpolation of lit lines
    interpolation_func interp_func = linear; //interpolation function to use for lit lines

    //blanking interpolation
    float blank_interp_distance = BLANK_INTERP_DEFAULT; //max distance for interpolation of blanking jumps
    interpolation_func blank_interp_func = linear;      //interpolation function to use for blanking jumps

private:
    OptimizerInternals* internal;
};

} // namespace lzr
