
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

	Frame line;
	line.add(Point(-1.0, 0.0, 255, 255, 255, 255));
	line.add(Point(-0.5, 0.0, 255, 255, 255, 255));

	frame = line;
	translate(line, 1.5, 0.0);
	frame.add_with_blank_jump(line);

	Optimizer opt;
	opt.run(frame);

	print_frame(frame);
}


int main()
{
	test_blanking_interpolation();
	return 0;
}
