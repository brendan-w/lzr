
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

#include "dac.h"


using namespace lzr;

typedef struct etherdream etherdream;
typedef struct etherdream_point etherdream_point;


static void* zmq_ctx;
static void* zmq_sub;
static Frame frame;



//main laser client
int main()
{
    int rc       = 0;
    zmq_ctx      = zmq_ctx_new();
    zmq_sub      = frame_sub_new(zmq_ctx, LZRD_GRAPHICS_ENDPOINT);

    init_dacs();

    DACList dacs;
    while(dacs.size() == 0)
    {
        //printf("Searching for Etherdream...\n");
        sleep(1); //wait for the etherdream lib to see pings from the dacs
        dacs = list_dacs();
    }

    for(std::string& name : dacs)
        std::cout << name << std::endl;

    DAC* dac = dac_connect(dacs[0]);
    std::cout << "Connected to: " << dac->name() << std::endl;

    //enter the main loop
    //-------------------
    while(1)
    {
        recv_frame(zmq_sub, frame);
        dac->send(frame);
    }
    //-------------------

    delete dac;

    zmq_close(zmq_sub);
    zmq_ctx_term(zmq_ctx);

    return rc;
}
