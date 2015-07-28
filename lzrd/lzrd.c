
/*
    Right now, this is mostly just glue that puts the laser
    on a ZMQ subscriber socket.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <lzr.h>
#include <lzr_zmq.h>
#include "liboptimize/include/lzr_optimize.h"
#include "libetherdream/etherdream.h"


typedef struct etherdream etherdream;
typedef struct etherdream_point etherdream_point;


static void*       zmq_ctx;
static void*       rx;
static lzr_frame*  frame;
static etherdream* dac;


static void send_frame()
{
    printf("RECV frame (%d points)\n", frame->n_points);
    
    etherdream_point* points = (etherdream_point*) calloc(sizeof(etherdream_point), frame->n_points);
    
    for(size_t i = 0; i < frame->n_points; i++)
    {
        points[i].x = frame->points[i].x;
        points[i].y = frame->points[i].y;
        points[i].r = frame->points[i].r;
        points[i].g = frame->points[i].g;
        points[i].b = frame->points[i].b;
        points[i].i = frame->points[i].i;
    }

    int rc = etherdream_write(dac, points, frame->n_points, 30000, -1);
    printf("SEND frame (%d points)\n", rc);
    etherdream_wait_for_ready(dac);

    free(points);
}


static int loop()
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
                send_frame();
                break;
        }
    }
}



//main laser client
int main()
{
    int rc  = 0;
    zmq_ctx = zmq_ctx_new();
    rx      = lzr_create_rx(zmq_ctx);
    frame   = (lzr_frame*) malloc(sizeof(lzr_frame));

    etherdream_lib_start();

    //discover DACs
    int dac_count = 0;
    while(dac_count == 0)
    {
        printf("Searching for Etherdream...\n");
        sleep(1);
        dac_count = etherdream_dac_count();
    }

    printf("Found %d Etherdream(s)...\n", dac_count);
    printf("Connecting to Etherdream...\n");

    dac = etherdream_get(0);
    rc = etherdream_connect(dac);
    assert(rc == 0);

    printf("Connection successful...\n");

    //enter the main loop
    //-------------------
    rc = loop(rx, frame);
    //-------------------

    etherdream_stop(dac);
    etherdream_disconnect(dac);
    free(frame);
    zmq_close(rx);
    zmq_ctx_destroy(zmq_ctx);

    return rc;
}
