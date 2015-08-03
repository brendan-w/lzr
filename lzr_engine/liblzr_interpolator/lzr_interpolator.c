
#include <stdlib.h>
#include <stdbool.h>
#include "lzr_interpolator.h"


typedef struct {
    lzr_frame frame;
    size_t max_distance;
} interp_t;


lzr_interpolator* lzr_interpolator_create()
{
    interp_t* interp = malloc(sizeof(interp_t));
    interp->max_distance = 0;
    return (lzr_interpolator*) interp;
}

void lzr_interpolator_destroy(lzr_interpolator* _interp)
{
    interp_t* interp = (interp_t*) _interp;
    free(interp);
}

void lzr_interpolator_set(lzr_interpolator* _interp, interp_property prop, int value)
{
    interp_t* interp = (interp_t*) _interp;
    switch(prop)
    {
        case LZR_INTERP_MAX_DISTANCE: interp->max_distance = (size_t) value; break;
    }
}

int lzr_interpolator_get(lzr_interpolator* _interp, interp_property prop)
{
    interp_t* interp = (interp_t*) _interp;
    switch(prop)
    {
        case LZR_INTERP_MAX_DISTANCE: return (int) interp->max_distance;
    }

    return 0;
}

int lzr_interpolate(lzr_interpolator* _interp, lzr_frame* frame)
{
    interp_t* interp = (interp_t*) _interp;

    //save the frame to the working buffer
    interp->frame = *frame;

    bool in_path = false;
    lzr_point prev;

    for(size_t i = 0; i < interp->frame.n_points; i++)
    {
        lzr_point p = interp->frame.points[i];

        if(LZR_POINT_IS_BLANKED(p))
        {
            in_path = false;
        }
        else if(!in_path)
        {
            prev = p;
            in_path = true;
        }
        else
        {
            //interpolate
        }
    }

    return 0;
}
