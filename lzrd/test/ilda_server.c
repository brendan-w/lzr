
#include <stdio.h>
#include <stdlib.h>

#include <lzr.h>


//test laser server
int main()
{
    void* zmq_ctx = lzr_create_zmq_ctx();
    void* tx      = lzr_create_frame_tx(zmq_ctx, LZR_ZMQ_ENDPOINT);
    void* interp  = lzr_interpolator_create();

    usleep(1200000);

    //create the working frame
    lzr_frame* frame = (lzr_frame*) malloc(sizeof(lzr_frame));

    

    printf("%zu\n", frame->n_points);

    for(size_t i = 0; i < frame->n_points; i++)
    {
        printf("(%f, %f)\n", frame->points[i].x, frame->points[i].y);
    }

    int i = 0;
    printf("sending...\n");
    while(1)
    {

        lzr_send_frame(tx, frame);
        usleep(1000000 / 300);
        i++;
    }

    free(frame);
    lzr_interpolator_destroy(interp);
    lzr_destroy_frame_tx(tx);
    lzr_destroy_zmq_ctx(zmq_ctx);

    return 0;
}
