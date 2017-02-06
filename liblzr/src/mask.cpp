
#include <math.h>
#include <liblzr.hpp>

namespace lzr {


//off-the-shelf ray casting test
static bool point_in_mask(Point& p, const Frame& mask)
{
    bool in = false;
    size_t i = 0;
    size_t j = 0;

    for(i = 0, j = mask.size() - 1; i < mask.size(); j = i++)
    {
        Point a = mask[i];
        Point b = mask[j];

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

void close_mask(Frame& mask)
{
    if(mask.front() != mask.back())
    {
        mask.add(mask.front());
    }
}

/*
 * Algorithm:
 *  - find all intersections between the frame and the mask, and
 *    insert a new point at the intersection.
 *  - test each line segment in the new frame, and perform a
 *    ray-cast test for whether it's within the mask.
 *  - blank points that fall outside the mask
 *  - remove redundant blanked points.
 */
int mask(Frame& frame, Frame mask, bool inverse)
{
    //bail early if there aren't any mask points
    if(frame.empty())
        return LZR_SUCCESS;

    if(mask.size() < 3)
        return LZR_ERROR_INVALID_ARG;

    // makes sure that we have all our line segments for testing
    close_mask(mask);

    //setup a buffer to build the finished product
    //TODO: rewrite this to operate in place
    Frame output;

    //since we operating in pairs
    output.add(frame[0]);

    // First pass, create points for all intersections
    // with the mask.
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
                //crossed the mask
                //add the point, with a copied color
                intersection.r = frame[i - 1].r;
                intersection.g = frame[i - 1].g;
                intersection.b = frame[i - 1].b;
                intersection.i = frame[i - 1].i;
                output.add(intersection);
            }

            output.add(frame[i]);
        }
    }

    /*
     * Second pass, test each line segment for being in/out of the mask
     * NOTE: we can't do this during the point insertion above, because
     * there are cases where a single segment will cross the mask multiple
     * times. Consider the arch-shaped mask below:
     *     ________
     *    |  ____  |
     * +--|-|----|-|---+
     *    |_|    |_|
     */

    for(size_t i = 1; i < frame.size(); i++)
    {
        //get a test point in the middle of the path
        Point mid = output[i - 1].lerp_to(output[i], 0.5);

        //if this line segment is inside the mask, shut it off
        //(using != as a logical XOR)
        if(inverse != point_in_mask(mid, mask))
        {
            output[i - 1].blank();
        }
    }

    /*
     * Third pass. Discard the probably-absurd quantity of blanked points
     */

    // TODO

    frame = output;
    return LZR_SUCCESS;
}

} // namespace lzr
