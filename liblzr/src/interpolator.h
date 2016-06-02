
#pragma once

#include <liblzr.h>

namespace lzr {


//generates extra points on a line
//only adds the interior points, not the endpoints
int interp_line(Frame& working, double max_distance, interpolation_func func, Point start, Point end);


} // namespace lzr

