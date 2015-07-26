

#include <stdlib.h>
#include <stdio.h>

#include <lzr.h>
#include <lzr_zmq.h>


//main laser client
int main()
{
    void* zmq_ctx = zmq_ctx_new();
    void* rx      = lzr_create_rx(zmq_ctx);

    //create the working frame
    lzr_frame* frame = (lzr_frame*) malloc(sizeof(lzr_frame));

    while(1)
    {
        lzr_recv_frame(rx, frame);

        printf("--------Frame-------\n");

        for(int i = 0; i < frame->n_points; i++)
        {
            printf("x=%d\n", frame->points[i].x);
        }
    }

    free(frame);
    zmq_close(rx);
    zmq_ctx_destroy(zmq_ctx);

    return 0;
}
