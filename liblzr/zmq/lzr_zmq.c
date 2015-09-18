
#include <zmq.h>
#include <assert.h>
#include <string.h>
#include <lzr.h>


/*
    socket creators
*/

//create a ZMQ transmitter (publisher)
void* lzr_create_frame_tx(void* zmq_ctx, char* address)
{
    void* publisher = zmq_socket(zmq_ctx, ZMQ_PUB);
    int rc = zmq_bind(publisher, address);
    assert(rc == 0);
    return publisher;
}


//create a ZMQ reciever (subscriber)
void* lzr_create_frame_rx(void* zmq_ctx, char* address)
{
    void* subscriber = zmq_socket(zmq_ctx, ZMQ_SUB);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, NULL, 0);
    zmq_connect(subscriber, address);
    return subscriber;
}


/*
    frame handlers
*/

//send a single frame
int lzr_send_frame(void* tx, lzr_frame* frame)
{
    //TODO: error checking
    //size of the point buffer in bytes
    size_t len = sizeof(lzr_point) * frame->n_points;

    //run the ZMQ send
    return zmq_send(tx, (void*)(frame->points), len, 0);
}



static int recv_frame(void* rx, lzr_frame* frame, int flags)
{
    //TODO: error checking

    //max number of bytes for the max number of points in a frame
    size_t max_len = (LZR_FRAME_MAX_POINTS * sizeof(lzr_point));

    //recieve and block
    int n = zmq_recv(rx, (void*) (frame->points), max_len, flags);

    //record the number of points sent
    frame->n_points = (uint16_t) (n / sizeof(lzr_point));

    return n;
}

//recieve a single frame (blocking)
int lzr_recv_frame(void* rx, lzr_frame* frame)
{
    return recv_frame(rx, frame, 0);
}

//recieve a single frame (NON blocking)
int lzr_recv_frame_no_block(void* rx, lzr_frame* frame)
{
    return recv_frame(rx, frame, ZMQ_DONTWAIT);
}
