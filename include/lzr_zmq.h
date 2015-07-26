
#ifndef LZR_ZMQ_H
#define LZR_ZMQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lzr.h>
#include <zmq.h>
#include <assert.h>
#include <string.h>

/*
    all ZMQ data is sent in two parts (messages).
    The first is a type code, defined below,
    and the second is the corresponding data.

    generally looks something like:

    lzr_send_topic(tx, LZR_ZMQ_TERMINATE);
    lzr_send_empty(tx);
*/

//ZMQ topics (ints)
#define LZR_ZMQ_ERROR -1
#define LZR_ZMQ_TERMINATE 0
#define LZR_ZMQ_FRAME 1
//...

// the default LZR endpoint
#ifndef LZR_ZMQ_ENDPOINT
#define LZR_ZMQ_ENDPOINT "tcp://127.0.0.1:5555"
#endif


/*
    socket creators
*/

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

/*
    topic handlers
*/

//send a topic header
static int lzr_send_topic(void* tx, int topic)
{
    return zmq_send(tx, &topic, sizeof(int), ZMQ_SNDMORE);
}

//get the topic header
static int lzr_recv_topic(void* rx)
{
    int topic = -1;
    zmq_recv(rx, (void*) (&topic), sizeof(int), 0);
    return topic;
}


/*
    general data senders
*/

static int lzr_send_empty(void* tx)
{
    return zmq_send(tx, NULL, 0, 0);
}


/*
    frame handlers
*/

//send a single frame
static int lzr_send_frame(void* tx, lzr_frame* frame)
{
    //TODO: error checking
    //size of the point buffer in bytes
    size_t len = sizeof(lzr_point) * frame->n_points;

    //run the ZMQ send
    return zmq_send(tx, (void*)(frame->points), len, 0);
}


//recieve a single frame
static void lzr_recv_frame(void* rx, lzr_frame* frame)
{
    //TODO: error checking

    //max number of bytes for the max number of points in a frame
    size_t max_len = (FRAME_MAX_POINTS * sizeof(lzr_point));

    //recieve and block
    int n = zmq_recv(rx, (void*) (frame->points), max_len, 0);

    //record the number of points sent
    frame->n_points = (uint16_t) (n / sizeof(lzr_point));
}


#ifdef __cplusplus
} // extern "c"
#endif

#endif /* LZR_ZMQ_H */
