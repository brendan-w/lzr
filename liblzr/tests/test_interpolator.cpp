
#include <stdio.h>
#include <assert.h>
#include <lzr.h>


static void print_frame(Frame& frame)
{
    for(Point& p : frame)
    {
        printf("(%f, %f, r=%d, g=%d, b=%d, i=%d)\n", p.x, p.y, p.r, p.g, p.b, p.i);
    }
}


static int test_single_line()
{
    Frame frame;
    frame.add(Point(-1.0, -1.0, 100, 100, 100, 100));
    frame.add(Point( 1.0, 1.0, 200, 200, 200, 200));

    Frame target_frame;
    target_frame.add(Point(-1.0, -1.0, 100, 100, 100, 100));
    target_frame.add(Point(-0.5, -0.5, 125, 125, 125, 125));
    target_frame.add(Point( 0.0,  0.0, 150, 150, 150, 150));
    target_frame.add(Point( 0.5,  0.5, 175, 175, 175, 175));
    target_frame.add(Point( 1.0,  1.0, 200, 200, 200, 200));

    interpolate(frame, 1.0, linear);

    // print_frame(frame);
    assert(frame.size() == 5);
    assert(frame == target_frame);

    return 0;
}


int main()
{
    test_single_line();
    return 0;
}
