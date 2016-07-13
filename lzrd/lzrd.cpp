
/*
    Right now, this is mostly just glue that puts an
    etherdream on a ZMQ subscriber socket.
*/

#include <unistd.h>
#include <liblzr.h>
#include <zmq.h>
#include <libconfig.h++>

#include "dac.h"

using namespace lzr;

static void* zmq_ctx;
static void* zmq_sub;
static Frame frame;
static DACList dacs;


//main laser client
int main()
{
    zmq_ctx = zmq_ctx_new();
    zmq_sub = frame_sub_new(zmq_ctx, LZRD_GRAPHICS_ENDPOINT);

    init_dacs();

    while(dacs.size() == 0)
    {
        sleep(1); //wait for the etherdream lib to see pings from the dacs
        dacs = list_dacs();
    }

    for(auto name : dacs)
        std::cout << name << std::endl;

    DAC* dac = dac_connect(dacs[0]);

    if(!dac)
    {
        std::cout << "Failed to connect" << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "Connected to: " << dac->name() << std::endl;
    }

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

    return 0;
}
