
#ifndef LZR_INTERPOLATE_H
#define LZR_INTERPOLATE_H

#include <lzr.h>


typedef void lzr_interpolator;

typedef enum {
    LZR_INTERP_MAX_DISTANCE,
} interp_property;


/*
    Allocates and returns a point to a new interpolator context.
*/
lzr_interpolator* lzr_interpolator_create();

/*
    Deallocator for the interpolator context
*/
void lzr_interpolator_destroy(lzr_interpolator* interp);


//settings modifier
void lzr_interpolator_set(lzr_interpolator* _opt, interp_property prop, int value);

//settings getter
int lzr_interpolator_get(lzr_interpolator* _opt, interp_property prop);

/*
    Main interpolator function.

    params:
        interp : the interpolator context
        frame  : the frame of points to be interpolated

    returns:
        0  : success
        -1 : error, not enough room for additional points
        -2 : error, LZR_INTERP_MAX_DISTANCE was set to zero
*/
int lzr_interpolator_run(lzr_interpolator* interp, lzr_frame* frame);


#endif /* LZR_INTERPOLATE_H */
