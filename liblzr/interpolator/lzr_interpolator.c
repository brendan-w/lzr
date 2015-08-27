
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <lzr.h>


#define DEFAULT_MAX_DISTANCE (LZR_POINT_MAX_POSITION / 100.0);


typedef struct {
    lzr_frame frame;
    double max_distance;
} interp_t;




lzr_interpolator* lzr_interpolator_create()
{
    interp_t* interp = malloc(sizeof(interp_t));
    interp->max_distance = DEFAULT_MAX_DISTANCE;
    return (lzr_interpolator*) interp;
}

void lzr_interpolator_destroy(lzr_interpolator* _interp)
{
    interp_t* interp = (interp_t*) _interp;
    free(interp);
}

//remove the value casting macro
#undef lzr_interpolator_set
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
    return 0;
}

static double lerp(double v0, double v1, double t)
{
    return (1-t)*v0 + t*v1;
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
            lzr_point p = start; //load color information into the new point

            double t = (double) i / n;
            p.x = lerp(start.x, end.x, t);
            p.y = lerp(start.y, end.y, t);

            //prevent multiple points at the same location
            if(!LZR_POINTS_SAME_POS(prev, p) &&
               !LZR_POINTS_SAME_POS(end, p))
            {
                if(add_point(interp, p))
                    return -1;

                prev = p;
            }
        }
    }

    return 0;
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
            int error = lerp_lzr(interp, prev, p);
            if(error)
                return error;
        }

        prev = p;
        if(add_point(interp, p))
            return LZR_ERROR_TOO_MANY_POINTS;
    }

    //write the finished frame to the output buffer
    *frame = interp->frame;
    return LZR_SUCCESS;
}
