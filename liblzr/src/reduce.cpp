
#include <liblzr.hpp>


namespace lzr {


int reduce_blanks(Frame& frame)
{
    //setup a buffer to build the finished product
    //TODO: rewrite this to operate in place
    Frame output;

    Point prev;
    bool was_lit = true;
    for(const Point& point : frame)
    {
        if(point.is_lit())
        {
            if(!was_lit)
            {
                output.add(prev);
            }

            output.add(point);
            was_lit = true;
        }
        else
        {
            was_lit = false;
        }

        prev = point;
    }

    frame = output;
    return LZR_SUCCESS;
}

int remove_duplicates(Frame& frame)
{
    //setup a buffer to build the finished product
    //TODO: rewrite this to operate in place
    Frame output;

    Point prev(0.0, 0.0, 0, 0, 0, 0); // initialize as blanked

    for(const Point& point : frame)
    {
        const bool discard = prev.is_lit() &&
                             point.is_lit() &&
                             point.same_position_as(prev);

        if(!discard)
        {
            output.add(point);
        }

        prev = point;
    }

    frame = output;
    return LZR_SUCCESS;
}


} // namespace lzr
