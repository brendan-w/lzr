
#include <cmath>
#include <liblzr.hpp>

namespace lzr {


/*
    The LZR coordinate system is [-1.0, 1.0] (2.0 units wide),
    and Laser DACs are usually 16 bit, so:

    2.0 / 65536 = 0.0000305 units per DAC step

    call the points equal if we're inside a half step:

    (2.0 / 65536) / 2.0 = 0.0000152
*/
#define FLOAT_EQUAL_TOLERANCE 0.0000152


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

void Point::set_position(const Point& other)
{
    x = other.x;
    y = other.y;
}

void Point::set_color(const Point& other)
{
    r = other.r;
    g = other.g;
    b = other.b;
    i = other.i;
}

bool Point::is_blanked() const
{
    return (i == 0) || (r + g + b == 0);
}

bool Point::is_lit() const
{
    return (i > 0) && (r + g + b > 0);
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

Point Point::lerp_to(const Point& other, double t) const
{
    Point p(lerp(x, other.x, t),
            lerp(y, other.y, t),
            lerp_uint8(r, other.r, t),
            lerp_uint8(g, other.g, t),
            lerp_uint8(b, other.b, t),
            lerp_uint8(i, other.i, t));
    return p;
}

double Point::sq_distance_to(const Point& other) const
{
    return (x - other.x)*(x - other.x) + (y - other.y)*(y - other.y);
}

bool Point::same_position_as(const Point& other) const
{
    return ((std::abs(x - other.x) <= FLOAT_EQUAL_TOLERANCE) &&
            (std::abs(y - other.y) <= FLOAT_EQUAL_TOLERANCE));
}

bool Point::same_color_as(const Point& other) const
{
    return ((r == other.r) &&
            (g == other.g) &&
            (b == other.b) &&
            (i == other.i));
}

bool Point::operator==(const Point& other) const
{
    return (same_position_as(other) &&
            same_color_as(other));
}

bool Point::operator!=(const Point& other) const
{
    return !operator==(other);
}


} // namespace lzr
