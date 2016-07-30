
#include <stdio.h>
#include <stdlib.h>

#include <lzr.h>
#include <zmq.h>



void calc_frame(lzr_frame* frame)
{
    //
    frame->points[0].x = -1.0;
    frame->points[0].y = -1.0;
    frame->points[0].r = 255;
    frame->points[0].g = 0;
    frame->points[0].b = 0;
    frame->points[0].i = 255;
    //red
    frame->points[1].x = 1.0;
    frame->points[1].y = -1.0;
    frame->points[1].r = 0;
    frame->points[1].g = 255;
    frame->points[1].b = 0;
    frame->points[1].i = 255;
    //green
    frame->points[2].x = 1.0;
    frame->points[2].y = 1.0;
    frame->points[2].r = 0;
    frame->points[2].g = 0;
    frame->points[2].b = 255;
    frame->points[2].i = 255;
    //blue
    frame->points[3].x = -1.0;
    frame->points[3].y = 1.0;
    frame->points[3].r = 255;
    frame->points[3].g = 0;
    frame->points[3].b = 255;
    frame->points[3].i = 255;
    //magenta
    frame->points[4].x = -1.0;
    frame->points[4].y = -1.0;
    frame->points[4].r = 255;
    frame->points[4].g = 0;
    frame->points[4].b = 0;
    frame->points[4].i = 255;

    frame->n_points = 5;
}


//test laser server
int main()
{
    void* zmq_ctx = zmq_ctx_new();
    void* zmq_pub = lzr_frame_pub(zmq_ctx, LZR_ZMQ_ENDPOINT);
    void* interp  = lzr_interpolator_create();

    usleep(1200000);

    //create the working frame
    lzr_frame* frame = (lzr_frame*) malloc(sizeof(lzr_frame));

    calc_frame(frame);
    // lzr_interpolator_set(interp, LZR_INTERP_MAX_DISTANCE, 150);
    int r = lzr_interpolator_run(interp, frame);
    if(r != LZR_SUCCESS)
    {
        printf("%d\n", r);
        return EXIT_FAILURE;
    }
    printf("%zu\n", frame->n_points);

    for(size_t i = 0; i < frame->n_points; i++)
    {
        printf("(%f, %f)\n", frame->points[i].x, frame->points[i].y);
    }

    int i = 0;
    printf("sending...\n");
    while(1)
    {

        lzr_send_frame(zmq_pub, frame);
        usleep(1000000 / 300);
        i++;
    }

    free(frame);
    lzr_interpolator_destroy(interp);
    zmq_close(zmq_pub);
    zmq_ctx_term(zmq_ctx);

    return 0;
}
