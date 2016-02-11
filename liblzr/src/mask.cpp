
#include <math.h>
#include <lzr.h>

namespace lzr {


typedef struct {

} Line;




//off-the-shelf ray casting test
static bool point_in_polygon(Point p, const Frame polygon)
{
    bool in = false;
    size_t i = 0;
    size_t j = 0;

    for(i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++)
    {
        Point a = polygon[i];
        Point b = polygon[j];

        if( ((a.y > p.y) != (b.y > p.y)) &&
            (p.x < (b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x) )
            in = !in;
    }

    return in;
}


// http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/1968345#1968345
// returns true if line segments intersect. If true, "output" will be set to the intersection point.
static bool get_line_intersection(const Point& a1,
                                  const Point& a2,
                                  const Point& b1,
                                  const Point& b2,
                                  Point& output)
{
    Point s1, s2;

    s1.x = a2.x - a1.x;
    s1.y = a2.y - a1.y;
    
    s2.x = b2.x - b1.x;
    s2.y = b2.y - b1.y;

    float s, t;
    s = (-s1.y * (a1.x - b1.x) + s1.x * (a1.y - b1.y)) / (-s2.x * s1.y + s1.x * s2.y);
    t = ( s2.x * (a1.y - b1.y) - s2.y * (a1.x - b1.x)) / (-s2.x * s1.y + s1.x * s2.y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        output.x = a1.x + (t * s1.x);
        output.y = a1.y + (t * s1.y);
        return true;
    }

    return false; // No collision
}


int mask(Frame& frame, const Frame& mask, bool inverse)
{
    //bail early if there aren't any mask points
    if(frame.empty())
        return LZR_SUCCESS;

    //setup a buffer to build the finished product
    Frame output;

    for(size_t i = 1; i < frame.size(); i++)
    {
        for(size_t m = 1; m < mask.size(); m++)
        {
            Point intersection;
            if(get_line_intersection(frame[i - 1],
                                     frame[i],
                                     mask[m - 1],
                                     mask[m],
                                     intersection))
            {
                //TODO
            }
        }
    }

    return LZR_SUCCESS;
}


} // namespace lzr
