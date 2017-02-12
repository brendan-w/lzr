
#pragma once

#include <liblzr.hpp>

namespace lzr {


//generates extra points on a line
//only adds the interior points, not the endpoints
int interp_line(Frame& working, double max_distance, interpolation_func func, Point start, Point end);

//interpolates only lit lines. Uses the function above
int interpolate(Frame& frame, double max_distance, interpolation_func func);

} // namespace lzr

