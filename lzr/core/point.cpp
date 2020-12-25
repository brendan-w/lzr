#include <cmath>

#include "lzr/core/core.hpp"

namespace lzr {

constexpr float Point::POSITION_MIN;
constexpr float Point::POSITION_MAX;
constexpr uint8_t Point::COLOR_MIN;
constexpr uint8_t Point::COLOR_MAX;

//run-of-the-mill linear interpolation
static inline float lerp(float v0, float v1, float t)
{
    return (1-(t))*(v0) + (t)*(v1);
}

//wrapper for lerping a uint8_t
static inline uint8_t lerp_uint8(uint8_t v0, uint8_t v1, float t)
{
    return (uint8_t) round(lerp((float) v0, (float) v1, t));
}

Point Point::lerp_to(const Point& other, float t) const
{
    return Point(lerp(x, other.x, t),
                 lerp(y, other.y, t),
                 lerp_uint8(r, other.r, t),
                 lerp_uint8(g, other.g, t),
                 lerp_uint8(b, other.b, t),
                 lerp_uint8(i, other.i, t));
}

float Point::distance_to(const Point& other) const
{
    return std::sqrt(sq_distance_to(other));
}

float Point::sq_distance_to(const Point& other) const
{
    return (x - other.x)*(x - other.x) + (y - other.y)*(y - other.y);
}

bool Point::same_position_as(const Point& other, const float tolerance) const
{
    return ((std::abs(x - other.x) < tolerance) &&
            (std::abs(y - other.y) < tolerance));
}

bool Point::same_color_as(const Point& other) const
{
    return ((r == other.r) &&
            (g == other.g) &&
            (b == other.b) &&
            (i == other.i) &&
            (beam == other.beam));
}

} // namespace lzr
