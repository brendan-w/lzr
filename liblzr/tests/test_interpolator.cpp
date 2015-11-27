
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


static void test_xyrgb_line()
{
    Frame frame;
    frame.add(Point(-1.0, -1.0, 100, 100, 100, 100));
    frame.add(Point( 1.0, 1.0, 200, 200, 200, 200));

    Frame target;
    target.add(Point(-1.0, -1.0, 100, 100, 100, 100));
    target.add(Point(-0.5, -0.5, 125, 125, 125, 125));
    target.add(Point( 0.0,  0.0, 150, 150, 150, 150));
    target.add(Point( 0.5,  0.5, 175, 175, 175, 175));
    target.add(Point( 1.0,  1.0, 200, 200, 200, 200));

    interpolate(frame, 1.0, linear);

    // print_frame(frame);
    assert(frame.size() == 5);
    assert(frame == target);
}

static void test_interp_func()
{
    Frame orig;
    Frame frame;
    Frame target;

    // each interpolation function will run on this input
    orig.add(Point(-1.0, -1.0, 255, 255, 255, 255));
    orig.add(Point( 1.0, 1.0, 255, 255, 255, 255));

    // LINEAR ----------------------------------------------
    target.clear();
    target.add(Point(-1.0, -1.0, 255, 255, 255, 255));
    target.add(Point(-0.5, -0.5, 255, 255, 255, 255));
    target.add(Point( 0.0,  0.0, 255, 255, 255, 255));
    target.add(Point( 0.5,  0.5, 255, 255, 255, 255));
    target.add(Point( 1.0,  1.0, 255, 255, 255, 255));

    frame = orig;
    interpolate(frame, 1.0, linear);
    // print_frame(frame);
    assert(frame == target);

    // QUAD --------------------------------------------------
    target.clear();
    target.add(Point(-1.0,  -1.0,  255, 255, 255, 255));
    target.add(Point(-0.75, -0.75, 255, 255, 255, 255));
    target.add(Point( 0.0,   0.0,  255, 255, 255, 255));
    target.add(Point( 0.75,  0.75, 255, 255, 255, 255));
    target.add(Point( 1.0,   1.0,  255, 255, 255, 255));

    frame = orig;
    interpolate(frame, 1.0, quad);
    // print_frame(frame);
    assert(frame == target);

    // QUART --------------------------------------------------
    target.clear();
    target.add(Point(-1.0,    -1.0,    255, 255, 255, 255));
    target.add(Point(-0.9375, -0.9375, 255, 255, 255, 255));
    target.add(Point( 0.0,     0.0,    255, 255, 255, 255));
    target.add(Point( 0.9375,  0.9375, 255, 255, 255, 255));
    target.add(Point( 1.0,     1.0,    255, 255, 255, 255));

    frame = orig;
    interpolate(frame, 1.0, quart);
    // print_frame(frame);
    assert(frame == target);
}


int main()
{
    test_xyrgb_line();
    test_interp_func();
    return 0;
}
