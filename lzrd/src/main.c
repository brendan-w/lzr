

#include <stdlib.h>
#include <stdio.h>

#include <lzr.h>
#include <lzr_zmq.h>


void handle_frame(lzr_frame* frame)
{
    printf("--------Frame-------\n");

    for(int i = 0; i < frame->n_points; i++)
    {
        printf("x=%d\n", frame->points[i].x);
    }
}

int loop(void* rx, lzr_frame* frame)
{
    while(1)
    {
        switch(lzr_recv_topic(rx))
        {
            case LZR_ZMQ_ERROR:
                return 1;
            case LZR_ZMQ_TERMINATE:
                return 0;
            case LZR_ZMQ_FRAME:
                lzr_recv_frame(rx, frame);
                handle_frame(frame);
                break;
        }
    }
}

//main laser client
int main()
{
    void* zmq_ctx = zmq_ctx_new();
    void* rx      = lzr_create_rx(zmq_ctx);

    //create the working frame
    lzr_frame* frame = (lzr_frame*) malloc(sizeof(lzr_frame));

    //enter the main loop
    int rc = loop(rx, frame);

    free(frame);
    zmq_close(rx);
    zmq_ctx_destroy(zmq_ctx);

    return rc;
}
