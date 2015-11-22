
#include <lzr.h>


/*
 * Constructors
 */

Point::Point() :
    x(0.0),
    y(0.0),
    r(0),
    g(0),
    b(0),
    i(0)
{ }

Point::Point(double x, double y) :
    x(x),
    y(y),
    r(0),
    g(0),
    b(0),
    i(0)
{ }

Point::Point(double x, double y, uint8_t r, uint8_t g, uint8_t b, uint8_t i) :
    x(x),
    y(y),
    r(r),
    g(g),
    b(b),
    i(i)
{ }


/*
 * functions
 */

void Point::blank()
{
    i = 0;
}

void Point::unblank()
{
    i = 255;
}

bool Point::is_blanked()
{
    return (i == 255);
}

bool Point::equal_position(const Point& other)
{
    //TODO: use tolerance for comparing floating points
    return ((x == other.x) &&
            (y == other.y));
}

bool Point::equal_color(const Point& other)
{
    return ((r == other.r) &&
            (g == other.g) &&
            (b == other.b) &&
            (i == other.i));
}

bool Point::equal(const Point& other)
{
    return (equal_color(other) &&
            equal_position(other));
}

bool Point::operator==(const Point& other)
{
    return equal(other);
}

double Point::sq_distance_to(const Point& other)
{
    return (x - other.x)*(x - other.x) + (y - other.y)*(y - other.y);
}
