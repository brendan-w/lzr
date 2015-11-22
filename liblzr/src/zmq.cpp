
#include <assert.h>
#include <string.h>
#include <zmq.h>
#include <lzr.h>


/*
 * socket creators
 */

//create a ZMQ transmitter (publisher)
void* frame_pub_new(void* zmq_ctx, const char* address)
{
    void* publisher = zmq_socket(zmq_ctx, ZMQ_PUB);
    int rc = zmq_bind(publisher, address);
    assert(rc == 0);
    return publisher;
}


//create a ZMQ reciever (subscriber)
void* frame_sub_new(void* zmq_ctx, const char* address)
{
    void* subscriber = zmq_socket(zmq_ctx, ZMQ_SUB);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, NULL, 0);
    zmq_connect(subscriber, address);
    return subscriber;
}


/*
 * frame handlers
 */

//send a single frame
int send_frame(void* pub, Frame* frame)
{
    size_t len = sizeof(Point) * frame->size();

    zmq_msg_t msg;
    zmq_msg_init_size(&msg, len);
    memcpy(zmq_msg_data(&msg), (void*) frame->data(), len);

    //run the ZMQ send
    return zmq_sendmsg(pub, &msg, 0);
}


//recieve a single frame (blocking)
int recv_frame(void* sub, Frame* frame)
{
    int r;

    //recieve and block
    zmq_msg_t msg;
    r = zmq_msg_init (&msg);
    assert(r == 0);
    r = zmq_recvmsg(sub, &msg, 0);
    assert(r == 0);

    frame->resize(zmq_msg_size(&msg) / sizeof(Point));
    memcpy((void*) frame->data(), zmq_msg_data(&msg), zmq_msg_size(&msg));

    return r;
}
