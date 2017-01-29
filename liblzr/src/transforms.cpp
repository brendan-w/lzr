
#include <cmath>
#include <liblzr.hpp>

namespace lzr {


/*
    Rule of thumb:
    use `Point` classes for specifying actual 2D locations,
    and seperate XY vars for scalars.
*/

int translate(Frame& frame, double x, double y)
{
    for(Point& p : frame)
    {
        p.x += x;
        p.y += y;
    }

    return LZR_SUCCESS;
}


int rotate(Frame& frame, Point center, double theta)
{
    // x' = cos(theta) * (x-ox) - sin(theta) * (y-oy) + ox
    // y' = sin(theta) * (x-ox) + cos(theta) * (y-oy) + oy
    // where (ox, oy) is the axis/origin to rotate around

    //cache the sin and cos values
    double s = std::sin(theta);
    double c = std::cos(theta);

    for(Point& p : frame)
    {
        double x = p.x - center.x;
        double y = p.y - center.y;
        p.x = c*x - s*y + center.x;
        p.y = s*x + c*y + center.y;
    }

    return LZR_SUCCESS;
}


int scale(Frame& frame, Point center, double x, double y)
{
    for(Point& p : frame)
    {
        p.x = ((p.x - center.x) * x) + center.x;
        p.y = ((p.y - center.y) * y) + center.y;
    }

    return LZR_SUCCESS;
}


int mirror(Frame& frame, Point center, bool x, bool y)
{
    return scale(frame, center, (x ? -1.0 : 1.0), (y ? -1.0 : 1.0));
}


int dup_mirror(Frame& frame, Point center, bool x, bool y, bool blank)
{
    if(x)
    {
        Frame copy = frame;
        scale(copy, center, -1.0, 1.0);
        if(blank)
            frame.add_with_blank_jump(copy);
        else
            frame.add(copy);
    }

    if(y)
    {
        Frame copy = frame;
        scale(copy, center, 1.0, -1.0);
        if(blank)
            frame.add_with_blank_jump(copy);
        else
            frame.add(copy);
    }

    return LZR_SUCCESS;
}


int dup_linear(Frame& frame, Point offset, size_t n_dups, bool blank)
{
    //TODO: decide
    //n_dups--; //give a visually correct readout of the number

    //bail, if there's nothing to do
    if((frame.size() == 0) || (n_dups == 0))
        return LZR_SUCCESS;

    Frame copy = frame;

    //compute the offset for one duplication
    offset.x /= (double) n_dups;
    offset.y /= (double) n_dups;

    for(size_t i = 0; i < n_dups; i++)
    {
        translate(copy, offset.x, offset.y);

        if(blank)
            frame.add_with_blank_jump(copy);
        else
            frame.add(copy);
    }

    return LZR_SUCCESS;
}


int dup_radial(Frame& frame, Point center, size_t n_dups, double angle, bool blank)
{
    //TODO: decide
    //n_dups--; //give a visually correct readout of the number

    //bail, if there's nothing to do
    if((frame.size() == 0) || (n_dups == 0))
        return LZR_SUCCESS;

    Frame copy = frame;

    //compute the angular offset for one duplication
    angle /= n_dups;

    for(size_t i = 0; i < n_dups; i++)
    {
        rotate(copy, center, angle);

        if(blank)
            frame.add_with_blank_jump(copy);
        else
            frame.add(copy);
    }

    return LZR_SUCCESS;
}


} // namespace lzr
