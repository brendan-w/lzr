
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <lzr.h>

using namespace lzr;


//test laser server
int main()
{
    void* zmq_ctx = zmq_ctx_new();
    void* zmq_pub = frame_pub_new(zmq_ctx, LZRD_GRAPHICS_ENDPOINT);

    usleep(1200000);

    //open the ILDA file
    // ILDA* f = ilda_open("../../green_cyan_fan.ild", "r");
    ILDA* f = ilda_open("../../../Downloads/ildatest.ild", "r");
    assert(f != NULL);

    //make sure projector 0 has frames
    assert(ilda_frame_count(f, 0) > 0);

    //read the frames
    FrameList frames;
    ilda_read(f, 0, frames);

    //pick the first frame
    assert(frames.size() > 0);
    Frame frame = frames[0];

    //use the first frame
    printf("%zu\n", frame.size());
    for(Point p : frame)
    {
        printf("(%f, %f) r=%d, g=%d, b=%d, i=%d\n", p.x, p.y, p.r, p.g, p.b, p.i);
    }

    printf("sending...\n");
    while(1)
    {

        send_frame(zmq_pub, frame);
        usleep(1000000 / 300);
    }

    ilda_close(f);
    zmq_close(zmq_pub);
    zmq_ctx_term(zmq_ctx);

    return 0;
}
