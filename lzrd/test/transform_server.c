
#include <stdio.h>
#include <stdlib.h>

#include <lzr.h>
#include <zmq.h>



void init_frame(lzr_frame* frame)
{
    //
    frame->points[0].x = -0.1;
    frame->points[0].y = -0.1;
    frame->points[0].r = 255;
    frame->points[0].g = 0;
    frame->points[0].b = 0;
    frame->points[0].i = 255;
    //red
    frame->points[1].x = 0.1;
    frame->points[1].y = -0.1;
    frame->points[1].r = 0;
    frame->points[1].g = 255;
    frame->points[1].b = 0;
    frame->points[1].i = 255;
    //green
    frame->points[2].x = 0.1;
    frame->points[2].y = 0.1;
    frame->points[2].r = 0;
    frame->points[2].g = 0;
    frame->points[2].b = 255;
    frame->points[2].i = 255;
    //blue
    frame->points[3].x = -0.1;
    frame->points[3].y = 0.1;
    frame->points[3].r = 255;
    frame->points[3].g = 0;
    frame->points[3].b = 255;
    frame->points[3].i = 255;
    //magenta
    frame->points[4].x = -0.1;
    frame->points[4].y = -0.1;
    frame->points[4].r = 255;
    frame->points[4].g = 0;
    frame->points[4].b = 0;
    frame->points[4].i = 255;

    frame->n_points = 5;
}


//test laser server
int main()
{
    lzr_frame frame;
    void* zmq_ctx = zmq_ctx_new();
    void* zmq_pub = lzr_frame_pub(zmq_ctx, LZR_ZMQ_ENDPOINT);
    void* interp  = lzr_interpolator_create();

    usleep(1200000);

    init_frame(&frame);



    lzr_point dest;
    dest.x = -0.5;
    dest.y = 0.5;

    lzr_frame_move_to(&frame, dest, LZR_BOUNDING_BOX);

    dest.x = 0.0;
    dest.y = 0.0;

    lzr_frame_dup_radial(&frame, dest, 40, 3.14, true);

    int r = lzr_interpolator_run(interp, &frame);
    if(r != LZR_SUCCESS)
    {
        printf("%d\n", r);
        return EXIT_FAILURE;
    }

    printf("sending %zu points\n", frame.n_points);
    
    while(1)
    {
        lzr_send_frame(zmq_pub, &frame);
        usleep(1000000 / 300);
    }

    lzr_interpolator_destroy(interp);
    zmq_close(zmq_pub);
    zmq_ctx_term(zmq_ctx);

    return 0;
}
