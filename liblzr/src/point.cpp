
#include <math.h>
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
    return (i == 0);
}

//run-of-the-mill linear interpolation
static inline double lerp(double v0, double v1, double t)
{
    return (1-(t))*(v0) + (t)*(v1);
}

//wrapper for lerping a uint8_t
static inline uint8_t lerp_uint8(uint8_t v0, uint8_t v1, double t)
{
    return (uint8_t) round(lerp((double) v0, (double) v1, t));
}

Point Point::lerp_to(const Point& other, double t)
{
    Point p(lerp(x, other.x, t),
            lerp(y, other.y, t),
            lerp_uint8(r, other.r, t),
            lerp_uint8(g, other.g, t),
            lerp_uint8(b, other.b, t),
            lerp_uint8(i, other.i, t));
    return p;
}

double Point::sq_distance_to(const Point& other)
{
    return (x - other.x)*(x - other.x) + (y - other.y)*(y - other.y);
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

bool Point::operator==(const Point& other)
{
    return (equal_color(other) &&
            equal_position(other));
}

bool Point::operator!=(const Point& other)
{
    return !operator==(other);
}
