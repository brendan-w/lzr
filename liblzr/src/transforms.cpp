
#include <math.h>
#include <lzr.h>


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
    double s = sin(theta);
    double c = cos(theta);

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
