
#include <math.h>
#include <algorithm> //sort()

#include "lzr/core/core.hpp"

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


bool line_crosses_bounding_box(const Point& a, const Point& b,
                               const Point& box_min, const Point& box_max)
{
    // test the four edges of the bounding box
    Point _;
    return get_line_intersection(a, b, box_min, Point(box_min.x, box_max.y), _) ||
           get_line_intersection(a, b, box_min, Point(box_max.x, box_min.y), _) ||
           get_line_intersection(a, b, box_max, Point(box_min.x, box_max.y), _) ||
           get_line_intersection(a, b, box_max, Point(box_max.x, box_min.y), _);
}

int intersect(Frame& frame, Frame mask)
{
    // bail early if there aren't any mask points
    if(frame.empty())
        return LZR_SUCCESS;

    if(mask.size() < 3)
        return LZR_ERROR_INVALID_ARG;

    // close the mask
    // makes sure that we have all our line segments for testing
    if(mask.front() != mask.back())
    {
        mask.add(mask.front());
    }

    Point mask_min;
    Point mask_max;
    mask.bounding_box(mask_min, mask_max);

    // setup a buffer to build the finished product
    // TODO: rewrite this to operate in place
    Frame output;

    // since we're operating in pairs
    output.add(frame[0]);


    // create points for all intersections with the mask.
    for(size_t i = 1; i < frame.size(); i++)
    {
        // the line segment we're looking at
        Point& a = frame[i - 1];
        Point& b = frame[i];

        // do a basic bounding-box test, before we go iterating through
        // all line segments in the mask
        if(!line_crosses_bounding_box(a, b, mask_min, mask_max))
        {
            // list of the intersections this line segment generates
            Frame intersections;

            for(size_t m = 1; m < mask.size(); m++)
            {
                Point intersection;
                if(get_line_intersection(a, b, mask[m - 1], mask[m], intersection))
                {
                    //crossed the mask
                    //add the point, with a copied color
                    intersection.set_color(a);
                    intersections.add(intersection);
                }

            }

            // sort the intersections so that we scan them in the right order
            sort(intersections.begin(),
                 intersections.end(),
                 [a](const Point& ia, const Point& ib) -> bool
            {
                return a.sq_distance_to(ia) < a.sq_distance_to(ib); 
            });

            //TODO: de-dupe the new intersection points
            output.add(intersections);
        }

        output.add(frame[i]);
    }

    frame = output;
    return LZR_SUCCESS;
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
    /*
     * First pass: create points for all intersections with the mask.
     */
    int r = intersect(frame, mask);

    if(r != LZR_SUCCESS)
    {
        return r;
    }

    /*
     * Second pass, test each line segment for being in/out of the mask
     * NOTE: we can't do this during the point insertion above, because
     * there are cases where a single segment will cross the mask multiple
     * times. Consider the arch-shaped mask below:
     *     ________
     *    |  ____  |
     *    | |    | |
     * +--+-+----+-+---+
     *    |_|    |_|
     */
    for(size_t i = 1; i < frame.size(); i++)
    {
        //get a test point in the middle of the path
        Point mid = frame[i - 1].lerp_to(frame[i], 0.5);

        //if this line segment is inside the mask, shut it off
        //(using != as a logical XOR)
        if(inverse != point_in_mask(mid, mask))
        {
            frame[i].blank();
        }
    }

    /*
     * Third pass. Discard the probably-absurd quantity of blanked points
     */
    // reduce_blanks(frame);  // TODO: prune it ourselves

    return LZR_SUCCESS;
}

} // namespace lzr
