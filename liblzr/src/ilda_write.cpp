
#include <string.h> //memset() zeroing out struct
#include <liblzr.hpp>
#include "ilda.hpp"

namespace lzr {


static int write_point(ILDA* ilda, Point& point, bool is_last)
{
    //right now, this parser will only output 2D True-Color points
    ilda_point_2d_true p;

    p.x = (int16_t) INT16_MAX * point.x;
    p.y = (int16_t) INT16_MAX * point.y;
    p.r = point.r;
    p.g = point.g;
    p.b = point.b;
    p.status.blanked = point.is_blanked();
    p.status.last_point = is_last;

    //convert to big-endian
    htobe_2d(&p);
    fwrite((void*) &p, 1, sizeof(ilda_header), ilda->f);

    return LZR_SUCCESS;
}


static int write_frame(ILDA* ilda, Frame& frame, size_t i, size_t pd)
{
    //skip empty frames, since they signify the end of a file
    if(frame.size() == 0)
        return LZR_SUCCESS;

    //zero out a new header
    ilda_header h;
    memset(&h, 0, sizeof(ilda_header));

    //set header details
    h.number_of_records = (uint16_t) frame.size();
    h.projector_id      = (uint8_t)  pd;
    h.total_frames      = (uint16_t) ilda->projectors[pd].n_frames;
    h.frame_number      = (uint16_t) i;

    //convert to big-endian
    htobe_header(&h);

    //write the header
    fwrite((void*) &h, 1, sizeof(ilda_header), ilda->f);

    //write each point to the file
    for(Point& point : frame)
    {
        bool is_last = (point == frame.back());
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
    memcpy(h.ilda, "ILDA", 4);

    //write the header
    fwrite((void*) &h, 1, sizeof(ilda_header), ilda->f);
}


/*
    Main API function for writing frames
    The ilda_close() call will generate the null ending frame
*/
int ilda_write(ILDA* ilda, size_t pd, FrameList& frame_list)
{
    if(ilda == NULL)
        return LZR_FAILURE;

    //check that this file is open for writing
    if(ilda->read)
        return LZR_FAILURE;

    ilda->projectors[pd].n_frames = frame_list.size();

    for(size_t i = 0; i < frame_list.size(); i++)
    {
        int r = write_frame(ilda, frame_list[i], i, pd);
        if(r != LZR_SUCCESS)
            return r;
    }

    return LZR_SUCCESS;
}


} // namespace lzr
