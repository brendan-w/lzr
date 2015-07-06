

#ifndef REARRANGE_PATHS_H
#define REARRANGE_PATHS_H

#include "optimize.h"

/*
    Arranges the paths in the path buffer to minimize blank time,
    and route the laser in a sensical path.
*/
void rearrange_paths(lzr_optimizer* opt);


#endif /* REARRANGE_PATHS_H */
