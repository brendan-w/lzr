#define _GNU_SOURCE

#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <lzr.h>
#include <zmq.h>


#define CIRCLE_POINTS 600

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


uint16_t colorsin(float pos) {
    int res = (sin(pos) + 1) * 128;
    if (res < 0) return 0;
    if (res > 255) return 255;
    return res;
}

void calc_frame(lzr_frame* frame, float phase)
{
    for(int i = 0; i < CIRCLE_POINTS; i++)
    {
        lzr_point* pt = (frame->points + i);

        float ip = (float)i * 2.0 * M_PI / (float)CIRCLE_POINTS;
        float ipf = fmod(ip + phase, 2.0 * M_PI);;

        //pattern
        ip *= 3;
        float R = 5;
        float r = 3;
        float D = 5;

        pt->x = 2500 * ((R-r)*cos(ip + phase) + D*cos((R-r)*ip/r));
        pt->y = 2500 * ((R-r)*sin(ip + phase) - D*sin((R-r)*ip/r));

        //normalize to [-1, 1]
        pt->x /= 32768;
        pt->y /= 32768;

        //color
        pt->r = colorsin(ipf);
        pt->g = colorsin(ipf + (2.0 * M_PI / 3.0));
        pt->b = colorsin(ipf + (4.0 * M_PI / 3.0));
        pt->i = 255;
    }

    frame->n_points = CIRCLE_POINTS;  
}


//test laser server
int main()
{
    void* zmq_ctx = zmq_ctx_new();
    void* zmq_pub = lzr_frame_pub(zmq_ctx, LZR_ZMQ_ENDPOINT);

    usleep(1200000);

    //create the working frame
    lzr_frame* frame = (lzr_frame*) malloc(sizeof(lzr_frame));

    int i = 0;

    while(1)
    {
        calc_frame(frame, (float) i / 50);

        lzr_send_frame(zmq_pub, frame);
        printf("sent\n");
        usleep(1000000 / 300);
        i++;
    }

    free(frame);
    zmq_close(zmq_pub);
    zmq_ctx_term(zmq_ctx);

    return 0;
}
