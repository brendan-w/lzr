
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <lzr.h>


#define MAX_DISTANCE_DEFAULT (LZR_POINT_POSITION_MAX / 50.0);


typedef struct {
    lzr_frame frame;
    double max_distance;
} interp_t;




lzr_interpolator* lzr_interpolator_create()
{
    interp_t* interp = malloc(sizeof(interp_t));
    interp->max_distance = MAX_DISTANCE_DEFAULT;
    return (lzr_interpolator*) interp;
}


void lzr_interpolator_destroy(lzr_interpolator* _interp)
{
    interp_t* interp = (interp_t*) _interp;
    free(interp);
}


#undef lzr_interpolator_set //remove the value casting macro
void lzr_interpolator_set(lzr_interpolator* _interp, interp_property prop, unsigned long value)
{
    interp_t* interp = (interp_t*) _interp;
    switch(prop)
    {
        case LZR_INTERP_MAX_DISTANCE: interp->max_distance = (double) value; break;
    }
}


//returns success of failure due to frame size constraint
static int add_point(interp_t* interp, lzr_point p)
{
    if(interp->frame.n_points >= LZR_FRAME_MAX_POINTS)
        return LZR_ERROR_TOO_MANY_POINTS;

    interp->frame.points[interp->frame.n_points] = p;
    interp->frame.n_points++;
    return LZR_SUCCESS;
}


static int lerp_lzr(interp_t* interp, lzr_point start, lzr_point end)
{
    double sq_dist     = LZR_POINT_SQ_DISTANCE(start, end);
    double sq_max_dist = interp->max_distance * interp->max_distance;

    if(sq_dist > sq_max_dist)
    {
        //root everything back to actual values
        double dist     = sqrt(sq_dist);
        double max_dist = interp->max_distance;

        //interpolate
        //number of intersticial points to generate
        size_t n = (size_t) (dist / max_dist); //integer division provides flooring

        n += 2; //include the two endpoints, which already exist

        lzr_point prev = start;


        //loop through the intersticial points
        for(size_t i = 1; i < (n-1); i++)
        {
            double t = (double) i / n;
            lzr_point p = lzr_point_lerp(&start, &end, t);

            //prevent multiple points at the same location
            if(!LZR_POINTS_SAME_POS(prev, p) &&
               !LZR_POINTS_SAME_POS(end, p))
            {
                int r = add_point(interp, p);
                if(r != LZR_SUCCESS)
                    return r;

                prev = p;
            }
        }
    }

    return LZR_SUCCESS;
}


int lzr_interpolator_run(lzr_interpolator* _interp, lzr_frame* frame)
{
    interp_t* interp = (interp_t*) _interp;

    if(interp->max_distance == 0)
        return LZR_ERROR_INVALID_PROPERTY;

    //wipe the working buffer
    interp->frame.n_points = 0;

    bool in_path = false; //whether or not the previous point was a lit point
    lzr_point prev;

    for(size_t i = 0; i < frame->n_points; i++)
    {
        lzr_point p = frame->points[i];

        if(LZR_POINT_IS_BLANKED(p))
        {
            in_path = false;
        }
        else if(!in_path)
        {
            in_path = true;
        }
        else
        {
            int r = lerp_lzr(interp, prev, p);
            if(r != LZR_SUCCESS)
                return r;
        }

        prev = p;
        int r = add_point(interp, p);
        if(r != LZR_SUCCESS)
            return r;
    }

    //write the finished frame to the output buffer
    *frame = interp->frame;
    return LZR_SUCCESS;
}
