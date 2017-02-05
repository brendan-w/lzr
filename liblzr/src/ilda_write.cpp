
#include <string.h> //memset() zeroing out struct
#include <algorithm> //min(), max()
#include <liblzr.hpp>
#include "ilda.hpp"

// not in C++11, so stub this out for now
template<class T>
constexpr const T& clamp( const T& v, const T& lo, const T& hi )
{
    return std::min<T>(std::max<T>(v, lo), hi);
}


namespace lzr {

static int write_point(ILDA* ilda, Point& p, bool is_last)
{
    // right now, this writer will only output 2D True-Color points
    ilda_point_2d_true ilda_p;
    memset(&ilda_p, 0, sizeof(ilda_point_2d_true));

    ilda_p.x = (int16_t) INT16_MAX * clamp<double>(p.x, LZR_POSITION_MIN, LZR_POSITION_MAX);
    ilda_p.y = (int16_t) INT16_MAX * clamp<double>(p.y, LZR_POSITION_MIN, LZR_POSITION_MAX);
    ilda_p.r = p.r;
    ilda_p.g = p.g;
    ilda_p.b = p.b;
    ilda_p.status.blanked = p.is_blanked();
    ilda_p.status.last_point = is_last;

    //convert to big-endian
    htobe_2d(&ilda_p);
    fwrite((void*) &ilda_p, 1, sizeof(ilda_point_2d_true), ilda->f);

    return LZR_SUCCESS;
}


static int write_frame(ILDA* ilda, Frame& frame, size_t pd)
{
    //skip empty frames, since they signify the end of a file
    if(frame.size() == 0)
        return LZR_SUCCESS;

    //zero out a new header
    ilda_header h;
    memset(&h, 0, sizeof(ilda_header));
    memcpy(h.ilda, ILDA_MAGIC, sizeof(h.ilda));

    //set header details
    h.format            = FORMAT_5_2D_TRUE;
    h.number_of_records = (uint16_t) frame.size();
    h.projector_id      = (uint8_t)  pd;
    //h.total_frames      = (uint16_t) ilda->projectors[pd].n_frames;
    h.frame_number      = (uint16_t) ilda->projectors[pd].n_frames;

    //convert to big-endian
    htobe_header(&h);

    //write the header
    fwrite((void*) &h, 1, sizeof(ilda_header), ilda->f);

    //write each point to the file
    for(Point& point : frame)
    {
        bool is_last = (&point == &(frame.back()));
        write_point(ilda, point, is_last);
    }

    return LZR_SUCCESS;
}


//writes a header where number_of_records is zero
void write_closer(ILDA* ilda)
{
    //zero out a new header
    ilda_header h;
    memset(&h, 0, sizeof(ilda_header));
    memcpy(h.ilda, ILDA_MAGIC, sizeof(h.ilda));

    //write the header
    fwrite((void*) &h, 1, sizeof(ilda_header), ilda->f);

    // TODO: adjust total frame counts
}


/*
    Main API function for writing frames
    The ilda_close() call will generate the null ending frame
*/
int ilda_write(ILDA* ilda, size_t pd, FrameList& frame_list)
{
    if(ilda == NULL)
    {
        return LZR_FAILURE;
    }

    //check that this file is open for writing
    if(ilda->read)
    {
        ilda->error = "Trying to write to ILDA file that is open for reading";
        return LZR_FAILURE;
    }

    for(Frame& frame : frame_list)
    {
        int r = write_frame(ilda, frame, pd);
        if(r == LZR_SUCCESS)
        {
            ilda->projectors[pd].n_frames++;
        }
        else
        {
            return r;
        }
    }

    return LZR_SUCCESS;
}


} // namespace lzr
