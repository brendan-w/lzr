
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

bool Frame::operator==(const Frame& other)
{
    if(size() != other.size())
        return false;

    for(size_t i = 0; i < size(); i++)
    {
        if(at(i) != other[i])
            return false;
    }

    return true;
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
