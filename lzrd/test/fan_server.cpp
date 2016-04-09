
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <assert.h>

#include <lzr.h>

using namespace lzr;

#define BEAMS 8
#define WIDTH 1.7
#define N_BEAM_POINTS 8
#define N_BLANK_POINTS 9
#define COLOR 0, 255, 0, 255


//test laser server
int main()
{
    void* zmq_ctx = zmq_ctx_new();
    void* zmq_pub = frame_pub_new(zmq_ctx, LZRD_GRAPHICS_ENDPOINT);

    usleep(1200000);

    Frame frame;

    //build the fan

    //blank
    for(int b = 0; b < N_BLANK_POINTS; b++)
        frame.add(Point(-1.0, 1.0, 0, 0, 0, 0));

    //upper fan
    for(int i = 0; i < BEAMS; i++)
    {
        double x = ((WIDTH / (BEAMS - 1)) * i) - (WIDTH / 2);
        std::cout << x << std::endl;

        //beam
        for(int n = 0; n < N_BEAM_POINTS; n++)
            frame.add(Point(x, 1.0, COLOR));
    }

    //blank
    for(int b = 0; b < N_BLANK_POINTS; b++)
        frame.add(Point(1.0, 1.0, 0, 0, 0, 0));

    //blank
    for(int b = 0; b < N_BLANK_POINTS; b++)
        frame.add(Point(1.0, -1.0, 0, 0, 0, 0));

    //upper fan
    for(int i = 0; i < BEAMS; i++)
    {
        double x = ((WIDTH / (BEAMS - 1)) * i) - (WIDTH / 2);
        x *= -1.0;

        //beam
        for(int n = 0; n < N_BEAM_POINTS; n++)
            frame.add(Point(x, -1.0, COLOR));
    }

    //blank
    for(int b = 0; b < N_BLANK_POINTS; b++)
        frame.add(Point(-1.0, -1.0, 0, 0, 0, 0));


    interpolate(frame, INTERP_DEFAULT + 0.01);


    //use the first frame
    printf("%zu\n", frame.size());
    for(Point p : frame)
    {
        printf("(%f, %f) r=%d, g=%d, b=%d, i=%d\n", p.x, p.y, p.r, p.g, p.b, p.i);
    }

    printf("sending...\n");
    while(1)
    {

        send_frame(zmq_pub, frame);
        usleep(1000000 / 300);
    }

    zmq_close(zmq_pub);
    zmq_ctx_term(zmq_ctx);

    return 0;
}
