
#ifndef LZR_ZMQ_H
#define LZR_ZMQ_H



#include <lzr.h>
#include <zmq.h>
#include <assert.h>


// the default LZR endpoing
#ifndef LZR_ZMQ_ENDPOINT
#define LZR_ZMQ_ENDPOINT "tcp://127.0.0.1:5555"
#endif


//create a ZMQ transmitter (publisher)
static void* lzr_create_tx(void* zmq_ctx)
{
    void* publisher = zmq_socket(zmq_ctx, ZMQ_PUB);
    int rc          = zmq_bind(publisher, LZR_ZMQ_ENDPOINT);
    assert(rc == 0);
    return publisher;
}


//create a ZMQ reciever (subscriber)
static void* lzr_create_rx(void* zmq_ctx)
{
    void* subscriber = zmq_socket(zmq_ctx, ZMQ_SUB);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, NULL, 0);
    zmq_connect(subscriber, LZR_ZMQ_ENDPOINT);
    return subscriber;
}


//send a single frame
static int lzr_send_frame(void* tx, lzr_frame* frame)
{
    //TODO: error checking
    //size of the point buffer in bytes
    size_t len = sizeof(lzr_point) * frame->n_points;

    //run the ZMQ send
    lzr_point* first = (lzr_point*) frame->points;
    return zmq_send(tx, (void*) first, len, 0);
}


//recieve a single frame
static void lzr_recv_frame(void* rx, lzr_frame* frame)
{
    //TODO: error checking

    //max number of bytes for the max number of points in a frame
    size_t max_len = (FRAME_MAX_POINTS * sizeof(lzr_point));

    //recieve and block
    lzr_point* first = (lzr_point*) frame->points;
    int n = zmq_recv(rx, (void*) first, max_len, 0);

    //record the number of points sent
    frame->n_points = (uint16_t) (n / sizeof(lzr_point));
}



#endif /* LZR_ZMQ_H */
