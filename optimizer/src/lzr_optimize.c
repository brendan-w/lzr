

#include <lzr_optimize.h>
#include <internals.h>



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
    opt->max_points = max_points;
    opt->points     = calloc(max_points, sizeof(opt_point_t));
    opt->paths      = calloc(max_points, sizeof(opt_path_t));
    opt->n_points   = 0;
    opt->n_paths    = 0;

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
    //return compile_paths(opt, points); //updates the point buffer and generates blanking jumps
    return 0;
}
