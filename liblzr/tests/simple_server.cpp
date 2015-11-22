
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <lzr.h>


//test laser server
int main()
{
    void* zmq_ctx = zmq_ctx_new();
    void* zmq_pub = frame_pub_new(zmq_ctx, LZR_ZMQ_ENDPOINT);
    usleep(1200000);


    Frame frame(2);
    frame[0] = Point(1.0, 1.0, 0, 255, 0, 255);
    frame[1] = Point(-1.0, -1.0, 0, 255, 0, 255);

    printf("sending...\n");
    while(1)
    {

        send_frame(zmq_pub, frame);
        usleep(1000000 / 300);
    }

    zmq_close(zmq_pub);
    zmq_ctx_term(zmq_ctx);

    return 0;
}
