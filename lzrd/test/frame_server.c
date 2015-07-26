

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <lzr.h>
#include <lzr_zmq.h>


//test laser server
int main()
{
    void* zmq_ctx = zmq_ctx_new();
    void* tx      = lzr_create_tx(zmq_ctx);

    //create the working frame
    lzr_frame* frame = (lzr_frame*) malloc(sizeof(lzr_frame));

    //load it with some data
    frame->points[0].x = 0;
    frame->points[1].x = 1;
    frame->points[2].x = 2;
    frame->points[3].x = 100;
    frame->n_points = 4;


    for(int i = 0; i < 3; i++)
    {
        lzr_send_frame(tx, frame);
        printf("sent\n");
        sleep(1);
    }

    free(frame);
    zmq_close(tx);
    zmq_ctx_destroy(zmq_ctx);

    return 0;
}
