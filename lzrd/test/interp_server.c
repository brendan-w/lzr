#define _GNU_SOURCE

#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <lzr.h>
#include <zmq.h>



void calc_frame(lzr_frame* frame)
{
    //
    frame->points[0].x = -LZR_POINT_MAX_POSITION;
    frame->points[0].y = -LZR_POINT_MAX_POSITION;
    frame->points[0].r = LZR_POINT_MAX_COLOR;
    frame->points[0].g = 0;
    frame->points[0].b = 0;
    frame->points[0].i = LZR_POINT_MAX_COLOR;
    //
    frame->points[1].x = LZR_POINT_MAX_POSITION;
    frame->points[1].y = -LZR_POINT_MAX_POSITION;
    frame->points[1].r = LZR_POINT_MAX_COLOR;
    frame->points[1].g = 0;
    frame->points[1].b = 0;
    frame->points[1].i = LZR_POINT_MAX_COLOR;
    //
    frame->points[2].x = LZR_POINT_MAX_POSITION;
    frame->points[2].y = LZR_POINT_MAX_POSITION;
    frame->points[2].r = LZR_POINT_MAX_COLOR;
    frame->points[2].g = 0;
    frame->points[2].b = 0;
    frame->points[2].i = LZR_POINT_MAX_COLOR;
    //
    frame->points[3].x = -LZR_POINT_MAX_POSITION;
    frame->points[3].y = LZR_POINT_MAX_POSITION;
    frame->points[3].r = LZR_POINT_MAX_COLOR;
    frame->points[3].g = 0;
    frame->points[3].b = 0;
    frame->points[3].i = LZR_POINT_MAX_COLOR;
    //
    frame->points[4].x = -LZR_POINT_MAX_POSITION;
    frame->points[4].y = -LZR_POINT_MAX_POSITION;
    frame->points[4].r = LZR_POINT_MAX_COLOR;
    frame->points[4].g = 0;
    frame->points[4].b = 0;
    frame->points[4].i = LZR_POINT_MAX_COLOR;

    frame->n_points = 5;
}


//test laser server
int main()
{
    void* zmq_ctx = lzr_create_zmq_ctx();
    void* tx      = lzr_create_frame_tx(zmq_ctx, LZR_ZMQ_ENDPOINT);
    void* interp  = lzr_interpolator_create();

    usleep(1200000);

    //create the working frame
    lzr_frame* frame = (lzr_frame*) malloc(sizeof(lzr_frame));

    calc_frame(frame);
    lzr_interpolator_run(interp, frame);
    printf("%zu\n", frame->n_points);

    int i = 0;
    while(1)
    {

        lzr_send_frame(tx, frame);
        printf("sent\n");
        usleep(1000000 / 300);
        i++;
    }

    free(frame);
    lzr_interpolator_destroy(interp);
    lzr_destroy_frame_tx(tx);
    lzr_destroy_zmq_ctx(zmq_ctx);

    return 0;
}
