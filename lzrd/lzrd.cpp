
/*
    Right now, this is mostly just glue that puts an
    etherdream on a ZMQ subscriber socket.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>

#include <vector>
#include <liblzr.h>
#include <zmq.h>
#include "libetherdream/etherdream.h"

using namespace lzr;


#define CLAMP(d) ( fmin(fmax(d, -1.0), 1.0) )

typedef struct etherdream etherdream;
typedef struct etherdream_point etherdream_point;


static void* zmq_ctx;
static void* zmq_sub;
static Frame frame;
static etherdream* dac;
static std::vector<etherdream_point> ether_points;


static void send_frame()
{
    //if the laser is ready for another frame
    if(etherdream_is_ready(dac) == 1)
    {
        printf("RECV frame (%d points)\n", frame.size());

        ether_points.clear();

        for(Point p : frame)
        {
            //convert LZR point into etherdream point
            etherdream_point ep;

            ep.x = (int16_t) (CLAMP(p.x) * 32767);
            ep.y = (int16_t) (CLAMP(p.y) * 32767);
            ep.r = (uint16_t) (p.r * 255);
            ep.g = (uint16_t) (p.g * 255);
            ep.b = (uint16_t) (p.b * 255);
            ep.i = (uint16_t) (p.i * 255);

            ether_points.push_back(ep);
        }

        etherdream_write(dac, ether_points.data(), ether_points.size(), 15000, -1);
    }
    //else, dump the frame, an old one is still being drawn
    //TODO: ^ is this really a good idea? Could create a stutterring animation
}


//main laser client
int main()
{
    int rc       = 0;
    zmq_ctx      = zmq_ctx_new();
    zmq_sub      = frame_sub_new(zmq_ctx, LZRD_GRAPHICS_ENDPOINT);
    ether_points.clear();


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
        recv_frame(zmq_sub, frame);
        send_frame();
    }
    //-------------------

    etherdream_stop(dac);
    etherdream_disconnect(dac);

    zmq_close(zmq_sub);
    zmq_ctx_term(zmq_ctx);

    return rc;
}
