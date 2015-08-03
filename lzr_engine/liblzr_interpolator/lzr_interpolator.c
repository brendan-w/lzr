
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "lzr_interpolator.h"


#define DEFAULT_MAX_DISTANCE (LZR_POINT_MAX_POSITION / 512);


typedef struct {
    lzr_frame frame;
    size_t max_distance;
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

static int add_point(interp_t* interp, lzr_point p)
{
    if(interp->frame.n_points >= LZR_FRAME_MAX_POINTS)
        return -1;

    interp->frame.points[interp->frame.n_points] = p;
    interp->frame.n_points++;
    return 0;
}

static double lerp(double v0, double v1, double t)
{
    return (1-t)*v0 + t*v1;
}

int lzr_interpolator_run(lzr_interpolator* _interp, lzr_frame* frame)
{
    interp_t* interp = (interp_t*) _interp;

    //wipe the working buffer
    interp->frame.n_points = 0;

    bool in_path = false; //whether or not the previous point was a lit point
    lzr_point prev;
    size_t sq_max_dist = interp->max_distance * interp->max_distance;

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
            size_t sq_dist = LZR_POINT_SQ_DISTANCE(prev, p);
            if(sq_dist > sq_max_dist)
            {
                //interpolate

                //number of intersticial points to generate
                size_t n = sq_dist / sq_max_dist; //integer division provides flooring
                if(sq_dist % sq_max_dist == 0) n--; //correct for paths that evenly divide

                n += 2; //include the two endpoints, which arleady exist

                //loop through the intersticial points
                for(size_t i = 1; i < (n-1); i++)
                {
                    lzr_point new = prev; //load color information into the new point

                    double t = (double) i / n;
                    new.x = (int16_t) round(lerp((double) prev.x, (double) p.x, t));
                    new.y = (int16_t) round(lerp((double) prev.y, (double) p.y, t));

                    if(add_point(interp, new))
                        return -1;
                }
            }
        }

        prev = p;
        if(add_point(interp, p))
            return -1;
    }

    //write the finished frame to the output buffer
    *frame = interp->frame;
    return 0;
}
