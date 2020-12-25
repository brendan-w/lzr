#include "gsl/gsl_util"
#include "lzr/core/core.hpp"

namespace lzr {

int decimate(Frame& frame, const size_t beam_threshold)
{
    Frame output;

    Point prev;
    size_t stacked_point_count = 1;
    for (const Point& p : frame)
    {
        auto _ = gsl::finally([&]{ prev = p; });

        // unconditionally add the first point
        if (output.empty())
        {
            // Disard all blanked points at the beginning of the frame
            if (p.is_lit())
            {
                output.add(p);
            }
            // else, discard blanked point
            continue;
        }

        // skip all duplicate/stacked points
        if (output.back() == p)
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


        // skip all blanked points until we find the transition to "lit"
        if (p.is_blanked())
        {
            continue;  // "discard" blanked point (we may use the cached one in "prev" when another lit path comes around)
        }

        // If the previous point was blanked, and this one is lit, add that previous blanked point
        if (prev.is_blanked() && p.is_lit()) {
            output.add(prev);  // Add blanked point
        }

        output.add(p);  // Add lit point
    }

    frame = output;
    return LZR_SUCCESS;
}

} // namespace lzr
