

#include <stdio.h>

#include <lzr_optimize.h>
#include <internals.h>

#define MAX_POINTS 1000


static void opt_log(lzr_optimizer* _opt)
{
    opt_t* opt = (opt_t*) _opt;

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
    lzr_point points[MAX_POINTS];
    //                        x, y, r, g, b, i
    lzr_point p0 = POINT_INIT(0, 0, 1, 1, 1, 1); points[0] = p0;
    lzr_point p1 = POINT_INIT(0, 8, 1, 1, 1, 1); points[1] = p1;
    lzr_point p2 = POINT_INIT(0, 8, 1, 1, 1, 0); points[2] = p2;

    lzr_point p4 = POINT_INIT(1, 0, 1, 1, 1, 1); points[3] = p4;
    lzr_point p3 = POINT_INIT(1, 8, 1, 1, 1, 1); points[4] = p3;
    lzr_point p5 = POINT_INIT(1, 8, 1, 1, 1, 0); points[5] = p5;

    lzr_point p6 = POINT_INIT(2, 0, 1, 1, 1, 1); points[6] = p6;
    lzr_point p7 = POINT_INIT(2, 8, 1, 1, 1, 1); points[7] = p7;
    lzr_point p8 = POINT_INIT(2, 8, 1, 1, 1, 0); points[8] = p8;

    lzr_optimizer* opt = lzr_create_optimizer(MAX_POINTS);

    lzr_optimizer_set(opt, LZR_OPT_MAX_POINTS, 1000);
    lzr_optimizer_set(opt, LZR_OPT_ANCHOR_POINTS, 1);

    lzr_optimize(opt, points, 9);
    opt_log(opt);
    lzr_destroy_optimizer(opt);
    return 0;
}
