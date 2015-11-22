
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <lzr.h>


//test laser server
int main()
{
    void* zmq_ctx = zmq_ctx_new();
    void* zmq_pub = frame_pub_new(zmq_ctx, LZR_ZMQ_ENDPOINT);

    usleep(1200000);

    //open the ILDA file
    lzr_ilda_file* f = lzr_ilda_read("../../green_cyan_fan.ild");

    size_t frame_count = lzr_ilda_frame_count(f, 0);

    assert(frame_count > 0);

    lzr_frame* frames = (lzr_frame*) calloc(sizeof(lzr_frame), frame_count);

    //read the frames
    lzr_ilda_read_frames(f, 0, frames);

    //use the first frame
    lzr_frame frame = frames[0];

    printf("%zu\n", frame.n_points);
    for(size_t i = 0; i < frame.n_points; i++)
    {
        lzr_point p = frame.points[i];
        printf("(%f, %f) r=%d, g=%d, b=%d, i=%d\n", p.x, p.y, p.r, p.g, p.b, p.i);
    }

    int i = 0;
    printf("sending...\n");
    while(1)
    {

        lzr_send_frame(zmq_pub, &frame);
        usleep(1000000 / 300);
        i++;
    }

    free(frames);
    zmq_close(zmq_pub);
    zmq_ctx_term(zmq_ctx);

    return 0;
}
