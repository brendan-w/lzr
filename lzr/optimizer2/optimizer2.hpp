#pragma once

#include <unordered_map>

#include "gtest/gtest_prod.h"
#include "lzr/core/core.hpp"

namespace lzr
{

static constexpr float ANGLE_ANY = 100;  // something bigger than 2*pi

struct PointAngle
{
    size_t p;  // Index of a point in the frame
    float angle;  // radian angle of associated with this point and the next
};

struct PathTraversal
{
    PointAngle first;
    PointAngle last;
};

/**
 * Class representing a range of lit points that the laser must draw. Provides helper
 * functions for exploring all possible traversals of the path, be it a single point,
 * a line, or a cyclic shape.
 */
struct Path
{
    Path(const Frame& frame, size_t f, size_t l);

    // indicies for the first and last points in the path
    const size_t first_p;
    const size_t last_p;

    // Number of points in this path
    const size_t size;

    // whether this path is a cycle
    const bool cycle;

    /**
     * Returns the number of possible traversals of this paths. Use the indicies
     * [0,num_traversals()) as inputs to traversal().
     */
    size_t num_traversals() const;

    /**
     * Returns the entrance and exit points for the given traversal of this path.
     * The angle associated with each point represents the entrance/exit angles
     * for this path.
     */
    PathTraversal traversal(const Frame& frame, size_t t) const;

    /**
     * Selects traversal T, and inserts those points into the output frame.
     */
    void traverse(const Frame& frame, size_t t, Frame& output) const;

private:

    FRIEND_TEST(Optimizer2, PointByOffset);

    /**
     * Helper function for accessing points in a looping manner. Returns the normalized
     * (in bounds) point index for the given offset. For instance:
     *     point_by_offset(0) == first_p
     *     point_by_offset(1) == first_p + 1
     *     ...
     *     point_by_offset(-1) == last_p
     *     point_by_offset(-2) == last_p - 1
     *     ...
     */
    size_t point_by_offset(ssize_t point_offset) const;

    // Cache of traversals, since the search algorithm will be interested in
    // repeatedly checking various entry possibilities. This is marked mutable
    // because it doesn't unfluence the actual point range specified by this
    // struct. This is purely a lookaside buffer for the traversal() function.
    mutable std::unordered_map<size_t, PathTraversal> traversal_cache_;
};


class Optimizer2
{
public:
    //main optimizer function
    int run(const Point& current_laser_position, Frame& frame);

private:

    std::vector<Path> paths_;
};

} // namespace lzr
