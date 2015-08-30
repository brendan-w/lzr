

#include <stdio.h>

#include <lzr.h>
#include "../optimizer/lzr_optimizer.h" //used for reading internal state


#define POINT_INIT(_x, _y, _r, _g, _b, _i) { .x=_x, .y=_y, .r=_r, .g=_g, .b=_b, .i=_i }


static void opt_log(lzr_optimizer* _opt)
{
    opt_t* opt = (opt_t*) _opt;

    printf("\nPoint buffer:\n");
    for(size_t i = 0; i < opt->n_points; i++)
    {
        opt_point_t point = opt->points[i];
        lzr_point base_point  = point.base_point;
        printf("%zu: (%f, %f) i=%d a=%f\n", i, base_point.x, base_point.y, base_point.i, point.angle);
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
    lzr_frame frame;
    //                        x,   y,   r, g, b, i
    lzr_point p0 = POINT_INIT(0.0, 0.0, 1, 1, 1, 1); frame.points[0] = p0;
    lzr_point p1 = POINT_INIT(0.0, 1.0, 1, 1, 1, 1); frame.points[1] = p1;
    lzr_point p2 = POINT_INIT(0.0, 1.0, 1, 1, 1, 0); frame.points[2] = p2;

    lzr_point p3 = POINT_INIT(1.0, 0.0, 1, 1, 1, 1); frame.points[3] = p3;
    lzr_point p4 = POINT_INIT(1.0, 1.0, 1, 1, 1, 1); frame.points[4] = p4;
    lzr_point p5 = POINT_INIT(1.0, 1.0, 1, 1, 1, 0); frame.points[5] = p5;

    lzr_point p6 = POINT_INIT(0.5, 0.0, 1, 1, 1, 1); frame.points[6] = p6;
    lzr_point p7 = POINT_INIT(0.5, 1.0, 1, 1, 1, 1); frame.points[7] = p7;
    lzr_point p8 = POINT_INIT(0.5, 1.0, 1, 1, 1, 0); frame.points[8] = p8;

    frame.n_points = 9;

    lzr_optimizer* opt = lzr_optimizer_create();

    lzr_optimizer_set(opt, LZR_OPT_ANCHOR_POINTS, 1);

    lzr_optimizer_run(opt, &frame);
    opt_log(opt);
    lzr_optimizer_destroy(opt);
    return 0;
}
