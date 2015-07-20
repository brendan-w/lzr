
#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <lzr.h>


typedef void lzr_optimizer;

/*
    Allocates and returns a point to a new optimizer context.
*/
lzr_optimizer* lzr_create_optimizer(size_t max_points);

/*
    Deallocator for the optimizer context
*/
void lzr_destroy_optimizer(lzr_optimizer* opt);

/*
    Main optimizer function. Accepts an array of lzr_points as input,

    params:
           opt : the optimizer context
        points : pointer to an array of lzr_points
             n : length of the points array

    returns:
        The number of new points written to the array. Original data
        is overwritten.
*/
size_t lzr_optimize(lzr_optimizer* opt, lzr_point* points, size_t n);


#endif /* OPTIMIZE_H */
