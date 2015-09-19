
#include "lzr_optimizer.h"



//simple helper to load points into the working buffer
//the opposite transaction takes place in compile_paths.c
static void to_buffer(opt_t* opt, lzr_frame* frame)
{
    opt->n_points = frame->n_points;
    for(size_t i = 0; i < frame->n_points; i++)
        opt->points[i].base_point = frame->points[i];
}


lzr_optimizer* lzr_optimizer_create()
{
    opt_t* opt = malloc(sizeof(opt_t));

    zero_opt_point(&opt->last_known_point);
    opt->anchor_points = 1;
    opt->points        = calloc(LZR_FRAME_MAX_POINTS, sizeof(opt_point_t));
    opt->paths         = calloc(LZR_FRAME_MAX_POINTS, sizeof(opt_path_t));
    opt->n_points      = 0;
    opt->n_paths       = 0;

    return (lzr_optimizer*) opt;
}

void lzr_optimizer_destroy(lzr_optimizer* _opt)
{
    opt_t* opt = (opt_t*) _opt;
    free(opt->points);
    free(opt->paths);
    free(opt);
}

//remove the value casting macro
#undef lzr_optimizer_set
void lzr_optimizer_set(lzr_optimizer* _opt, opt_property prop, unsigned long value)
{
    opt_t* opt = (opt_t*) _opt;
    switch(prop)
    {
        case LZR_OPT_ANCHOR_POINTS: opt->anchor_points = (size_t) value; break;
    }
}

int lzr_optimizer_run(lzr_optimizer* _opt, lzr_frame* frame)
{
    opt_t* opt = (opt_t*) _opt;

    to_buffer(opt, frame);  //load the points into the working buffer
    find_paths(opt);            //populates the path buffer
    rearrange_paths(opt);       //sorts the path buffer
    return compile_paths(opt, frame); //updates the point buffer and generates blanking jumps
}
