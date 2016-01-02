
#include <assert.h>
#include <lzr.h>

using namespace lzr;


static void print_frame(Frame& frame)
{
    for(Point& p : frame)
    {
        printf("(%f, %f, r=%d, g=%d, b=%d, i=%d)\n", p.x, p.y, p.r, p.g, p.b, p.i);
    }
}


static void test_blanking_interpolation()
{
    Frame frame;
    Frame target;

    //two lone points, with a blank jump in between
    frame.add(                Point(0.5, 0.0, 255, 255, 255, 255));
    frame.add_with_blank_jump(Point(1.0, 0.0, 255, 255, 255, 255));

    target = frame;

    Optimizer opt;
    opt.run(frame);

    //make sure that the right points are blanked
    for(int i = 0; i <= 5; i++)
       assert(frame[i].is_blanked());

    assert(frame[6].is_lit());

    for(int i = 7; i <= 12; i++)
       assert(frame[i].is_blanked());

    assert(frame[13].is_lit());


    //check positioning
    assert(frame[0] == Point(0.0, 0.0, 0, 0, 0, 0));
    assert(frame[5].same_position_as(target[0]));
    assert(frame[7].same_position_as(target[0]));
    assert(frame[12].same_position_as(target[3]));
}


int main()
{
    test_blanking_interpolation();
    return 0;
}
