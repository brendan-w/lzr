
#include <lzr.h>

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
