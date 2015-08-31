
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <lzr.h>


//test laser server
int main()
{
    void* zmq_ctx = lzr_create_zmq_ctx();
    void* tx      = lzr_create_frame_tx(zmq_ctx, LZR_ZMQ_ENDPOINT);
    void* interp  = lzr_interpolator_create();

    usleep(1200000);

    //open the ILDA file
    lzr_ilda_file* f = lzr_ilda_read("../../../Downloads/ildatest.ild");

    size_t frame_count = lzr_ilda_frame_count(f, 0);

    assert(frame_count > 0);

    lzr_frame* frames = (lzr_frame*) calloc(sizeof(lzr_frame), frame_count);

    //read the frames
    lzr_ilda_get_frames(f, 0, frames);

    //use the first frame
    lzr_frame frame = frames[0];

    printf("%zu\n", frame.n_points);
    for(size_t i = 0; i < frame.n_points; i++)
    {
        printf("(%f, %f)\n", frame.points[i].x, frame.points[i].y);
    }

    int i = 0;
    printf("sending...\n");
    while(1)
    {

        lzr_send_frame(tx, &frame);
        usleep(1000000 / 300);
        i++;
    }

    free(frames);
    lzr_interpolator_destroy(interp);
    lzr_destroy_frame_tx(tx);
    lzr_destroy_zmq_ctx(zmq_ctx);

    return 0;
}
