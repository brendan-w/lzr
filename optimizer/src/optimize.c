
#include <stdio.h>

#include "optimize.h"

#include "find_paths.h"
#include "rearrange_paths.h"
#include "compile_paths.h"



static void to_buffer(lzr_optimizer* opt, lzr_point* points, size_t n)
{
    opt->n_points = n;
    for(size_t i = 0; i < n; i++)
        opt->points[i].base_point = points[i];
}

static void from_buffer(lzr_optimizer* opt, lzr_point* points)
{
    for(size_t i = 0; i < opt->n_points; i++)
        points[i] = opt->points[i].base_point;
}


/*
    Allocates and returns a point to a new optimizer context.
*/
lzr_optimizer* lzr_create_optimizer(size_t max_points)
{
    lzr_optimizer* opt = malloc(sizeof(lzr_optimizer));

    zero_opt_point(&opt->last_known_point);
    opt->max_points = max_points;
    opt->points     = calloc(max_points, sizeof(opt_point_t));
    opt->paths      = calloc(max_points, sizeof(opt_path_t));
    opt->n_points   = 0;
    opt->n_paths    = 0;

    return opt;
}

/*
    Deallocator for the optimizer context
*/
void lzr_destroy_optimizer(lzr_optimizer* opt)
{
    free(opt->points);
    free(opt->paths);
    free(opt);
}

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
size_t lzr_optimize(lzr_optimizer* opt, lzr_point* points, size_t n)
{
    to_buffer(opt, points, n); //load the points into the working buffer
    find_paths(opt);           //populates the path buffer
    // rearrange_paths(opt);      //sorts the path buffer
    // compile_paths(opt);        //updates the point buffer and generates blanking jumps
    from_buffer(opt, points);  //copy the working buffer back to the source
    return opt->n_points;      //return the new number of points in the frame
}












static void opt_log(lzr_optimizer* opt)
{
    printf("\nPoint buffer:\n");
    for(size_t i = 0; i < opt->n_points; i++)
    {
        opt_point_t point = opt->points[i];
        lzr_point base_point  = point.base_point;
        printf("%zu: (%d, %d) i=%d a=%f\n", i, base_point.x, base_point.y, base_point.i, point.angle);
    }

    printf("\nPath buffer:\n");
    for(size_t i = 0; i < opt->n_paths; i++)
    {
        opt_path_t path = opt->paths[i];
        printf("%zu: [%zu, %zu] c=%d\n", i, path.a, path.b, path.cycle);        
    }
}

int main()
{
    lzr_point points[9];
    //                        x, y, r, g, b, i
    /*
    lzr_point p0 = POINT_INIT(0, 0, 1, 1, 1, 1); points[0] = p0;
    lzr_point p1 = POINT_INIT(0, 8, 1, 1, 1, 1); points[1] = p1;
    lzr_point p2 = POINT_INIT(0, 8, 1, 1, 1, 0); points[2] = p2;

    lzr_point p4 = POINT_INIT(1, 0, 1, 1, 1, 1); points[3] = p4;
    lzr_point p3 = POINT_INIT(1, 8, 1, 1, 1, 1); points[4] = p3;
    lzr_point p5 = POINT_INIT(1, 8, 1, 1, 1, 0); points[5] = p5;

    lzr_point p6 = POINT_INIT(2, 0, 1, 1, 1, 1); points[6] = p6;
    lzr_point p7 = POINT_INIT(2, 8, 1, 1, 1, 1); points[7] = p7;
    lzr_point p8 = POINT_INIT(2, 8, 1, 1, 1, 0); points[8] = p8;
    */

    lzr_point p0 = POINT_INIT(1, 1, 1, 1, 1, 1); points[0] = p0;
    lzr_point p1 = POINT_INIT(8, 2, 1, 1, 1, 1); points[1] = p1;
    lzr_point p2 = POINT_INIT(3, 3, 1, 1, 1, 1); points[2] = p2;


    lzr_optimizer* opt = lzr_create_optimizer(1000);
    lzr_optimize(opt, points, 3);
    opt_log(opt);
    lzr_destroy_optimizer(opt);
    return 0;
}
