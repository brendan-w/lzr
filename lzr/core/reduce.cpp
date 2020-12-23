
#include "lzr/core/core.hpp"

namespace lzr {

int reduce_duplicates(Frame& frame)
{
    Point prev(0.0, 0.0, 0, 0, 0, 0); // initialize as blanked

    auto it = frame.begin();
    while(it != frame.end())
    {
        const Point& point = *it;
        const bool discard = prev.is_lit() &&
                             point.is_lit() &&
                             point.same_position_as(prev);
        prev = point;

        if(discard)
        {
            it = frame.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return LZR_SUCCESS;
}

int reduce_blanks(Frame& frame)
{
    //setup a buffer to build the finished product
    //TODO: rewrite this to operate in place
    Frame output;

    output.add(frame[0]);
    Point prev = frame[0];
    for (size_t i = 1; i < frame.size(); i++)
    {
        const Point& point = frame[i];
        if(point.is_lit())
        {
            if(prev.is_blanked())
            {
                output.add(prev);
            }

            output.add(point);
        }

        prev = point;
    }

    frame = output;
    return LZR_SUCCESS;
}

int reduce_interpolation(Frame& frame)
{
    (void) frame;
    return LZR_FAILURE;
}


} // namespace lzr
