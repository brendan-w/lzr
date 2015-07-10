

#ifndef OPTIMIZER_INTERNALS_H
#define OPTIMIZER_INTERNALS_H

#include <optimize.h>


/*
    Splits a point buffer into individual path segments.
    Loads the result into the given path buffer.
*/
void find_paths(lzr_optimizer* opt);

/*
    Arranges the paths in the path buffer to minimize blank time,
    and route the laser in a sensical path.
*/
void rearrange_paths(lzr_optimizer* opt);

/*
    Fills the given output buffer according to the modified path buffer.
    Inserts/generates new blanking jumps. Returns the new size of the
    output buffer.
*/
size_t compile_paths(lzr_optimizer* opt, lzr_point* output);


#endif /* OPTIMIZER_INTERNALS_H */
