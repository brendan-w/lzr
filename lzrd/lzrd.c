
/*
    Right now, this is mostly just glue that puts an
    etherdream on a ZMQ subscriber socket.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <lzr.h>
#include <zmq.h>
#include "libetherdream/etherdream.h"


typedef struct etherdream etherdream;
typedef struct etherdream_point etherdream_point;


static void*             zmq_ctx;
static void*             rx;
static lzr_frame*        frame;
static etherdream*       dac;
static etherdream_point* ether_points;


static void send_frame()
{
    //if the laser is ready for another frame
    if(etherdream_is_ready(dac) == 1)
    {
        printf("RECV frame (%d points)\n", frame->n_points);

        for(size_t i = 0; i < frame->n_points; i++)
        {
            ether_points[i].x = frame->points[i].x;
            ether_points[i].y = frame->points[i].y;
            ether_points[i].r = frame->points[i].r;
            ether_points[i].g = frame->points[i].g;
            ether_points[i].b = frame->points[i].b;
            ether_points[i].i = frame->points[i].i;
        }

        etherdream_write(dac, ether_points, frame->n_points, 30000, -1);
    }
    //else, dump the frame, an old one is still being drawn
    //TODO: ^ is this really a good idea? Could create a stutterring animation
}


//main laser client
int main()
{
    int rc       = 0;
    zmq_ctx      = lzr_create_zmq_ctx();
    rx           = lzr_create_frame_rx(zmq_ctx, LZR_ZMQ_ENDPOINT);
    frame        = (lzr_frame*) malloc(sizeof(lzr_frame));
    ether_points = (etherdream_point*) calloc(sizeof(etherdream_point), LZR_FRAME_MAX_POINTS);

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
    while(1)
    {
        lzr_recv_frame(rx, frame);
        send_frame();
    }
    //-------------------

    etherdream_stop(dac);
    etherdream_disconnect(dac);

    free(ether_points);
    free(frame);
    lzr_destroy_frame_rx(rx);
    lzr_destroy_zmq_ctx(zmq_ctx);

    return rc;
}
