
#include "optimizer2.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

namespace lzr
{
namespace
{
// Computes the angle from point 1 to point 2 on range (-PI, PI]
inline float angle_between(const Point& p1, const Point& p2)
{
    return std::atan2(p2.y - p1.y, p2.x - p1.y) * -1.0;
}

inline bool is_cycle(const Frame& frame, size_t f, size_t l)
{
    return f == l ? false : frame[f].same_position_as(frame[l]);
}

inline size_t positive_modulo(ssize_t i, ssize_t n)
{
    return ((i % n) + n) % n;
}
}  // anonymous namespace


Path::Path(const Frame& frame, size_t f, size_t l)
  : first_p(std::min(f, l))
  , last_p(std::max(f, l))
  // min() and max() above guarantees that first_p <= last_p
  , size((last_p - first_p) + 1)
  , cycle(is_cycle(frame, f, l))
{
    // allocate the memory needed to store all of our traversals
    traversal_cache_.reserve(size);
}

size_t Path::num_traversals() const
{
    if (size == 1)
    {
        return 1;
    }
    else if (cycle)
    {
        // cyclic paths are allowed to be traversed from any
        // starting point, both forwards and backwards.
        return (size - 1) * 2;
    }
    else
    {
        // Non-cyclic paths have exactly two ways that
        // they can be traversed (forwards and backwards).
        return 2;
    }
}

PathTraversal Path::traversal(const Frame& frame, size_t t) const
{
    // If we have a cached value, return it as-is, rather
    // than doing expensive angle computation again.
    if (traversal_cache_.count(t) > 0)
    {
        return traversal_cache_[t];
    }

    PathTraversal traversal;

    assert(t < num_traversals());

    if (size == 1)
    {
        traversal = {
            {first_p, ANGLE_ANY},
            {last_p, ANGLE_ANY}
        };
    }
    else if (cycle)
    {
        // In cyclic paths, our T-values come in pairs for each point, where the first
        // indicates forwards travel, and the second indicates backwards travel.
        bool forward = (t % 2 == 0);

        // Look up the actual point index in the frame
        size_t p = first_p + (t / 2);

        (void) forward;
        (void) p;

        // if (forward)
        // {
        // }

        // traversal = {
        //     {p, angle_between(frame[p], frame[second_p])},
        //     {p, angle_between(frame[penultimate_p], frame[p])}
        // };
    }
    else
    {
        // else, not a cycle, we have only two path choices for traversing a line
        if (t == 0)
        {
            // start at the first point
            traversal = {
                {first_p, angle_between(frame[first_p], frame[first_p + 1])},
                {last_p, angle_between(frame[last_p - 1], frame[last_p])}
            };
        }
        else
        {
            // start at the last point
            traversal = {
                {last_p, angle_between(frame[last_p], frame[last_p - 1])},
                {first_p, angle_between(frame[first_p + 1], frame[first_p])}
            };
        }
    }

    // cache the value so that we can recall it later
    traversal_cache_[t] = traversal;

    return traversal;
}

void Path::traverse(const Frame& frame, size_t t, Frame& output) const
{
    if (size == 1)
    {
        assert(t == 0);
        output.push_back(frame[first_p]);
    }
    else if (cycle)
    {
        // See the traversal() function above for why we carve up the T-space in the
        // way we do. Pairs of points represent a single starting/ending point,
        // in the forward and reverse cycle directions respectively.

        // TODO: deduplicate this logic with traversal()

        bool forward = (t % 2 == 0);

        // Look up the actual point index in the frame
        size_t p = first_p + (t / 2);

        // <= so that we get the initial point twice. Once at the beginning
        // and again at the end.
        for(size_t i = 0; i <= size; i++)
        {
            output.push_back(frame[p]);

            if (forward)
            {
                p = ((p == last_p) ? (first_p + 1) : (p + 1));
            }
            else
            {
                p = ((p == first_p) ? (last_p - 1) : (p - 1));
            }
        }
    }
    else
    {
        if (t == 0)
        {
            // Forwards
            for (size_t p = first_p; p <= last_p; p++)
            {
                output.push_back(frame[p]);
            }
        }
        else
        {
            // Backwards
            for (size_t p = last_p; p != first_p; p--)
            {
                output.push_back(frame[p]);
            }
            output.push_back(frame[first_p]);
        }
    }
}

size_t Path::point_by_offset(ssize_t point_offset) const
{
    return first_p + positive_modulo(point_offset, size);
}

} // namespace lzr
