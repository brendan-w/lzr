
#include <stdio.h>
#include <lzr.h>
#include "lzr_interpolator.h"


#define POINT_INIT(_x, _y, _r, _g, _b, _i) { .x=_x, .y=_y, .r=_r, .g=_g, .b=_b, .i=_i }


static void print_frame(lzr_frame* frame)
{
    for(size_t i = 0; i < frame->n_points; i++)
    {
        lzr_point p = frame->points[i];
        printf("%zu: (%d, %d)\n", i, p.x, p.y);
    }
}

int main()
{
    lzr_frame frame;

    //                        x,  y,  r, g, b, i
    lzr_point p0 = POINT_INIT(0,  0,  1, 1, 1, 1); frame.points[0] = p0;
    lzr_point p1 = POINT_INIT(10, 10, 1, 1, 1, 1); frame.points[1] = p1;
    lzr_point p2 = POINT_INIT(0,  0,  1, 1, 1, 0); frame.points[2] = p2;

    // lzr_point p4 = POINT_INIT(0,  0,  1, 1, 1, 1); frame.points[3] = p4;
    // lzr_point p3 = POINT_INIT(20, 20, 1, 1, 1, 1); frame.points[4] = p3;
    // lzr_point p5 = POINT_INIT(1,  8,  1, 1, 1, 0); frame.points[5] = p5;

    // lzr_point p6 = POINT_INIT(0,  0,  1, 1, 1, 1); frame.points[6] = p6;
    // lzr_point p7 = POINT_INIT(30, 30, 1, 1, 1, 1); frame.points[7] = p7;
    // lzr_point p8 = POINT_INIT(0,  0,  1, 1, 1, 0); frame.points[8] = p8;

    frame.n_points = 3;

    lzr_interpolator* interp = lzr_interpolator_create();

    lzr_interpolator_set(interp, LZR_INTERP_MAX_DISTANCE, 1);
    lzr_interpolator_run(interp, &frame);
    print_frame(&frame);
    lzr_interpolator_destroy(interp);
    return 0;
}

