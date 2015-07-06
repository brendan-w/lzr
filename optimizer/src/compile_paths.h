

#ifndef COMPILE_PATHS_H
#define COMPILE_PATHS_H

#include "optimize.h"

/*
    Rebuilds the point buffer according to the modified path buffer.
    Inserts/generates new blanking jumps.
*/
void compile_paths(lzr_optimizer* opt);


#endif /* COMPILE_PATHS_H */
