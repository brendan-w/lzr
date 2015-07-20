

#include <internals.h>


typedef struct {
    size_t n;
    lzr_point* points;
} blank_jump;

//generates a blanking jump between two 
static blank_jump blank_between(opt_point_t* a, opt_point_t* b)
{
    blank_jump jump;
    return jump;
}


size_t compile_paths(opt_t* opt, lzr_point* output)
{
    size_t n = 0;

    for(size_t i = 0; i < opt->n_paths; i++)
    {
        //if the last_known_point is different than this frames
        //starting point, then an introductory blanking jump
        //is neccessary.

        if(POINTS_EQUAL(opt->last_known_point.base_point, ))
        {

        }

    }

    return n;
}
