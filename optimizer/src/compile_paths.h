

#ifndef COMPILE_PATHS_H
#define COMPILE_PATHS_H

#include "optimize.h"

/*
    Rebuilds the point buffer according to the modified path buffer.
    Inserts/generates new blanking jumps.
*/
void compile_paths(opt_point_buffer* points, opt_path_buffer* paths);


#endif /* COMPILE_PATHS_H */
