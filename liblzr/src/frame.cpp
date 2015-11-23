
#include <math.h>
#include <vector>
#include <lzr.h>


/*
 * constructors
 */

Frame::Frame() : std::vector<Point>() { }
Frame::Frame(size_t n) : std::vector<Point>(n) { }


/*
 * operators
 */

Frame& Frame::operator+=(const Point& p)
{
    push_back(Point(p));
    return *this;
}

Frame& Frame::operator+=(const Frame& other)
{
    for(Point p : other)
    {
        push_back(Point(p));
    }

    return *this;
}


/*
 * functions
 */

int Frame::rotate(double x_axis, double y_axis, double theta)
{
    // x' = cos(theta) * (x-ox) - sin(theta) * (y-oy) + ox
    // y' = sin(theta) * (x-ox) + cos(theta) * (y-oy) + oy
    // where (ox, oy) is the axis/origin to rotate around

    //cache the sin and cos values
    double s = sin(theta);
    double c = cos(theta);

    for(Point& p : *this)
    {
        double x = p.x - x_axis;
        double y = p.y - y_axis;
        p.x = c*x - s*y + x_axis;
        p.y = s*x + c*y + y_axis;
    }

    return LZR_SUCCESS;
}

int Frame::translate(double x, double y)
{
    for(Point& p : *this)
    {
        p.x += x;
        p.y += y;
    }

    return LZR_SUCCESS;
}


int Frame::scale(double x, double y)
{
    for(Point& p : *this)
    {
        p.x *= x;
        p.y *= y;
    }

    return LZR_SUCCESS;
}


Point Frame::bounding_box_center()
{
    //edges
    double left = 1.0; //start these at opposite extremes
    double right = -1.0;
    double bottom = 1.0;
    double top = -1.0;

    for(Point& p : *this)
    {
        if(p.x < left)   left = p.x;
        if(p.x > right)  right = p.x;
        if(p.y < bottom) bottom = p.y;
        if(p.y > top)    top = p.y;
    }

    Point center;
    center.x = (left + right) / 2.0;
    center.y = (bottom + top) / 2.0;

    return center;
}


Point Frame::average_center()
{
    Point center;
    center.x = 0.0;
    center.y = 0.0;

    for(Point& p : *this)
    {
        center.x += p.x;
        center.y += p.y;
    }

    center.x /= (double) size();
    center.y /= (double) size();

    return center;
}
