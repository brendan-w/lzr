

#include <lzr_optimize.h>
#include <internals.h>



//simple helper to load points into the working buffer
//the opposite transaction takes place in compile_paths.c
static void to_buffer(opt_t* opt, lzr_point* points, size_t n)
{
    opt->n_points = n;
    for(size_t i = 0; i < n; i++)
        opt->points[i].base_point = points[i];
}


lzr_optimizer* lzr_create_optimizer(size_t max_points)
{
    opt_t* opt = malloc(sizeof(opt_t));

    zero_opt_point(&opt->last_known_point);
    opt->max_points    = max_points;
    opt->anchor_points = 1;
    opt->points        = calloc(max_points, sizeof(opt_point_t));
    opt->paths         = calloc(max_points, sizeof(opt_path_t));
    opt->n_points      = 0;
    opt->n_paths       = 0;

    return (lzr_optimizer*) opt;
}

void lzr_destroy_optimizer(lzr_optimizer* _opt)
{
    opt_t* opt = (opt_t*) _opt;
    free(opt->points);
    free(opt->paths);
    free(opt);
}


size_t lzr_optimize(lzr_optimizer* _opt, lzr_point* points, size_t n)
{
    opt_t* opt = (opt_t*) _opt;

    to_buffer(opt, points, n);  //load the points into the working buffer
    find_paths(opt);            //populates the path buffer
    rearrange_paths(opt);       //sorts the path buffer
    return compile_paths(opt, points); //updates the point buffer and generates blanking jumps
}


void lzr_optimizer_set(lzr_optimizer* _opt, opt_property prop, int value)
{
    opt_t* opt = (opt_t*) _opt;
    switch(prop)
    {
        case LZR_OPT_MAX_POINTS:    opt->max_points    = (size_t) value; break;
        case LZR_OPT_ANCHOR_POINTS: opt->anchor_points = (size_t) value; break;
    }
}

int lzr_optimizer_get(lzr_optimizer* _opt, opt_property prop)
{
    opt_t* opt = (opt_t*) _opt;
    switch(prop)
    {
        case LZR_OPT_MAX_POINTS:    return (int) opt->max_points;
        case LZR_OPT_ANCHOR_POINTS: return (int) opt->anchor_points;
    }

    return 0;
}
