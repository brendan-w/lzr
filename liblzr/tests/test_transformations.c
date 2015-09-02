
#include <stdio.h>
#include <lzr.h>


#define POINT_INIT(_x, _y, _r, _g, _b, _i) { .x=_x, .y=_y, .r=_r, .g=_g, .b=_b, .i=_i }


static void print_frame(lzr_frame* frame)
{
    for(size_t i = 0; i < frame->n_points; i++)
    {
        lzr_point p = frame->points[i];
        printf("%zu: (%f, %f)\n", i, p.x, p.y);
    }
}

int main()
{
    lzr_frame frame;

    //                        x,    y,    r, g, b, i
    lzr_point p0 = POINT_INIT(0.25,  0.25,  1, 1, 1, 1); frame.points[0] = p0;
    lzr_point p1 = POINT_INIT(-0.25, 0.25,  1, 1, 1, 1); frame.points[1] = p1;
    lzr_point p2 = POINT_INIT(-0.25, -0.25, 1, 1, 1, 1); frame.points[2] = p2;
    lzr_point p3 = POINT_INIT(0.25,  -0.25, 1, 1, 1, 1); frame.points[3] = p3;
    frame.n_points = 4;

    lzr_point offset = POINT_INIT(0.25, 0.25, 1, 1, 1, 1);
    int r = lzr_frame_dup_linear(&frame, offset, 3, false);

    print_frame(&frame);
    printf("frame transformation returned: %d\n", r);
    return 0;
}

