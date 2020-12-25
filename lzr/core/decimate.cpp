#include <cmath>
#include <optional>
#include <limits>

#include "gsl/gsl_util"
#include "lzr/core/core.hpp"

namespace lzr {
namespace {

static constexpr float COLLINEARITY_THRESHOLD = std::numeric_limits<float>::epsilon();

inline float triangle_area(const Point& p0, const Point& p1, const Point& p2)
{
    // shoelace formula
    return 0.5 * std::fabs((p0.x * p1.y) +
                           (p1.x * p2.y) +
                           (p2.x * p0.y) -
                           (p1.x * p0.y) -
                           (p2.x * p1.y) -
                           (p0.x * p2.y));
}

/**
 * Deletes consequtive stacked points (within a controllable radius) and intersticial
 * blanking points. Lit points that are perfectly stacked may set the "beam" boolean
 * to true (if enough lit points are stacked).
 */
void prune_blanks_and_beams(Frame& frame, const size_t beam_threshold)
{
    Frame output;

    Point prev;
    size_t stacked_point_count = 1;

    for (const Point& p : frame)
    {
        auto _ = gsl::finally([&]{ prev = p; });

        // skip all blanked points until we find the transition to "lit". When we come to
        // the end of a blank jump, we will actually recall this point via "prev".
        if (p.is_blanked())
        {
            continue;
        }

        /**
         * Initial point
         */

        if (output.empty())
        {
            output.add(p);  // Point is guaranteed to be lit because of the blank-test above.
            continue;
        }

        /**
         * Duplicate points
         */

        // skip all duplicate/stacked points
        if (output.back().same_position_as(p))
        {
            stacked_point_count++;
            // update the "beam" status on our point
            output.back().beam = (stacked_point_count >= beam_threshold);
            continue;  // discard point
        }
        else  // points are different. Cancel out stack count
        {
            stacked_point_count = 1;
        }

        /**
         * Blanked points
         */

        // If the previous point was blanked, and this one is lit, add that previous blanked point
        // as the destination of a blank jump.
        if (prev.is_blanked() && p.is_lit()) {
            output.add(prev);
        }

        output.add(p);  // Add lit point
    }

    frame = output;
}

/**
 * This function performs a collinear test on successive points in a lit path.
 * This is an O(n) operation, and is therefore wise to do before any more intensive
 * operations which may be higher complexity. Laser graphics often have a lot of
 * straight lines so it's worth an extra trip through the frame to prune as many
 * points as possible.
 */
void prune_collinear_points(Frame& frame)
{
    Frame output;

    std::optional<Point> first_point = std::nullopt;
    std::optional<Point> second_point = std::nullopt;  // the point directly following "first_point", which we'll use to test the collinearity of all successive points in the line

    Point prev;
    for (const Point& p : frame)
    {
        auto _ = gsl::finally([&]{ prev = p; });

        if (p.is_blanked()) {
            // if we had a line going, close it using the previous point
            if (second_point.has_value())
            {
                output.add(prev);
            }

            first_point = std::nullopt;
            second_point = std::nullopt;
            output.add(p);
            continue;
        }

        // accumulate a first and second point
        if (!first_point.has_value())
        {
            output.add(p);
            first_point = p;
            continue;
        }

        if (!second_point.has_value())
        {
            second_point = p;
            continue;
        }

        // Now that we have two points to define a line, test every successive point as a third
        const float area = triangle_area(first_point.value(), second_point.value(), p);
        if (area >= COLLINEARITY_THRESHOLD)
        {
            // this new point has deviated too far from the line, lay down our previous point, and start a new path
            output.add(prev);
            first_point = prev;
            second_point = p;
        }
        // else, discard the point (we may actually use it via "prev")
    }

    // If we were tracking a line, close it
    if (second_point.has_value())
    {
        output.add(prev);
    }

    frame = output;
}
}  // anonymous namespace

int decimate(Frame& frame, const size_t beam_threshold)
{
    // First, we toss all blanks, and look for perfectly stacked points that would produce
    // beams. It's important to capture the beam information early, before successive
    // operations prune these redundant points.
    prune_blanks_and_beams(frame, beam_threshold);

    // Toss interpolation of straight lines. This can trim thousands of points in some
    // images, which speeds up any remaining decimation steps considerably.
    prune_collinear_points(frame);
    return LZR_SUCCESS;
}

} // namespace lzr
