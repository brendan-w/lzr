
#include <string.h> //strcmp() for "ILDA" header
#include <lzr.h>
#include "ilda.h"

namespace lzr {


// function signiture for point readers
typedef int (*point_reader)(ILDA* ilda, Point* p);



//reads a single record, and error checks the size
static int read_record(ILDA* ilda, void* buffer, size_t buffer_size)
{
    size_t r = fread(buffer, 1, buffer_size, ilda->f);

    //make sure we got everything...
    if(r != buffer_size)
    {
        ilda->error = "Encountered incomplete record";
        return ILDA_ERROR;
    }

    return ILDA_CONTINUE;
}


/*
 * Record readers/decoders
 * These functions translate ILDA structs into LZR structs
 *
 * Point's are returned in the `output` param
 * Colors are placed in that projector's color palette array
 */

// ================================ Format 0 ================================
static int read_3d_indexed(ILDA* ilda, Point* p)
{
    ilda_point_3d_indexed ilda_p;

    int r = read_record(ilda, (void*) &ilda_p, sizeof(ilda_point_3d_indexed));
    if(STATUS_IS_HALTING(r)) return r;

    //convert the ILDA point to a Point
    betoh_3d(&ilda_p);

    p->x = (double) ilda_p.x / INT16_MAX;
    p->y = (double) ilda_p.y / INT16_MAX;
    // ilda_p.z is completely discarded (orthographically projected)
    ilda_color c = ilda->current_projector()->lookup_color(ilda_p.color);
    p->r = c.r;
    p->g = c.g;
    p->b = c.b;
    p->i = UINT8_MAX;
    if(ilda_p.status.blanked)
        p->r = p->g = p->b = p->i = 0;

    return ILDA_CONTINUE;
}


// ================================ Format 1 ================================
static int read_2d_indexed(ILDA* ilda, Point* p)
{
    ilda_point_2d_indexed ilda_p;

    int r = read_record(ilda, (void*) &ilda_p, sizeof(ilda_point_2d_indexed));
    if(STATUS_IS_HALTING(r)) return r;

    //convert the ILDA point to a Point
    betoh_2d(&ilda_p);

    p->x = (double) ilda_p.x / INT16_MAX;
    p->y = (double) ilda_p.y / INT16_MAX;
    ilda_color c = ilda->current_projector()->lookup_color(ilda_p.color);
    p->r = c.r;
    p->g = c.g;
    p->b = c.b;
    p->i = UINT8_MAX;
    if(ilda_p.status.blanked)
        p->r = p->g = p->b = p->i = 0;

    return ILDA_CONTINUE;
}


// ================================ Format 2 ================================
static int read_colors(ILDA* ilda)
{
    size_t n = ilda->h.number_of_records;

    ilda->current_projector()->clear_palette();

    for(size_t i = 0; i < n; i++)
    {
        ilda_color c;

        //read one color record
        int r = read_record(ilda, (void*) &c, sizeof(ilda_color));
        if(STATUS_IS_HALTING(r)) return r;

        //store the new color
        ilda->current_projector()->add_color_to_palette(c);
    }

    return ILDA_CONTINUE;
}


// ================================ Format 3 ================================
static int read_3d_true(ILDA* ilda, Point* p)
{
    ilda_point_3d_true ilda_p;

    int r = read_record(ilda, (void*) &ilda_p, sizeof(ilda_point_3d_true));
    if(STATUS_IS_HALTING(r)) return r;

    //convert the ILDA point to a Point
    betoh_3d(&ilda_p);

    //read the point data
    p->x = (double) ilda_p.x / INT16_MAX;
    p->y = (double) ilda_p.y / INT16_MAX;
    // ilda_p.z is completely discarded (orthographically projected)
    p->r = ilda_p.r;
    p->g = ilda_p.g;
    p->b = ilda_p.b;
    p->i = UINT8_MAX;

    if(ilda_p.status.blanked)
        p->r = p->g = p->b = p->i = 0;

    return ILDA_CONTINUE;
}

// ================================ Format 4 ================================
static int read_2d_true(ILDA* ilda, Point* p)
{
    ilda_point_2d_true ilda_p;

    int r = read_record(ilda, (void*) &p, sizeof(ilda_point_2d_true));
    if(STATUS_IS_HALTING(r)) return r;

    //convert the ILDA point to a Point
    betoh_2d(&ilda_p);

    //read the point data
    p->x = (double) ilda_p.x / INT16_MAX;
    p->y = (double) ilda_p.y / INT16_MAX;
    p->r = ilda_p.r;
    p->g = ilda_p.g;
    p->b = ilda_p.b;
    p->i = UINT8_MAX;

    if(ilda_p.status.blanked)
        p->r = p->g = p->b = p->i = 0;

    return ILDA_CONTINUE;
}





/*

    Generic frame reading function

    saves decoded ILDA points off to the user's buffer.
    pass function pointer to the neccessary decoder.

*/
static int read_frame(ILDA* ilda, FrameList& frame_list, point_reader read_point)
{
    int status = ILDA_CONTINUE;

    //lookup the number of frames
    size_t n = ilda->h.number_of_records;

    if(n == 0)
    {
        ilda->error = "Found frame with no points.";
        status = ILDA_WARN;
    }

    //if there's already a problem, return early
    if(STATUS_IS_HALTING(status)) return status;

    //allocate them all at once
    Frame frame(n);

    //iterate over the records
    for(size_t i = 0; i < n; i++)
    {
        Point point;
        int r = read_point(ilda, &point);
        if(STATUS_IS_HALTING(r)) return r;

        //save the new point to the user's buffer
        frame[i] = point;

        //TODO: listen to the status byte for end-of-frame
        //if this was the last point, stop
        // if(p.status.last_point)
        // {
        //     frame.resize(i + 1);
        //     break;
        // }
    }

    //add the frame to the user's animation
    frame_list.push_back(frame);

    return status;
}


/*
 * Header Reader
 */
static int read_header(ILDA* ilda)
{
    //read one ILDA header
    size_t r = fread((void*) &(ilda->h),
                     1,
                     sizeof(ilda_header),
                     ilda->f);

    //did we capture a full header?
    if(r != sizeof(ilda_header))
    {
        if(r > 0)
            ilda->error = "Encountered incomplete ILDA section header";

        return ILDA_ERROR;
    }

    //is it prefixed with "ILDA"?
    if(strcmp("ILDA", ilda->h.ilda) != 0)
    {
        ilda->error = "Section header did not contain \"ILDA\"";
        return ILDA_ERROR;
    }

    //correct for the big-endianness of the file
    betoh_header(&(ilda->h));

    return ILDA_CONTINUE;
}


//reads a single section (frame) of the ILDA file
//returns boolean for whether to continue parsing
static int read_section_for_projector(ILDA* ilda, uint8_t pd, FrameList& frame_list)
{
    int status;

    //pull out the next header
    status = read_header(ilda);
    if(STATUS_IS_HALTING(status)) return status;

    //this section contains no records, halt parsing
    if(ilda->h.number_of_records == 0)
        return ILDA_HALT;

    //if this section isn't marked for projector we're looking for, skip it 
    if(ilda->h.projector_id != pd)
        return ILDA_CONTINUE;

    //invoke the corresponding parser for this section type
    switch(ilda->h.format)
    {
        case 0:
            status = read_frame(ilda, frame_list, read_3d_indexed); break;
        case 1:
            status = read_frame(ilda, frame_list, read_2d_indexed); break;
        case 2:
            status = read_colors(ilda); break;
        case 4:
            status = read_frame(ilda, frame_list, read_3d_true); break;
        case 5:
            status = read_frame(ilda, frame_list, read_2d_true); break;
        default:
            /*
                In this case, we can't skip past an unknown
                section type, since we don't know the record size.
            */
            status = ILDA_ERROR;
    }

    return status;
}



/*
    Call this AFTER reading in a header. If you decide you aren't interested
    in the data within that frame, call this to skip to the next header.
*/
static bool skip_to_next_section(ILDA* ilda)
{
    int skip_bytes = 0;

    switch(ilda->h.format)
    {
        case 0: skip_bytes = sizeof(ilda_point_3d_indexed); break;
        case 1: skip_bytes = sizeof(ilda_point_2d_indexed); break;
        case 2: skip_bytes = sizeof(ilda_color);            break;
        case 4: skip_bytes = sizeof(ilda_point_3d_true);    break;
        case 5: skip_bytes = sizeof(ilda_point_2d_true);    break;
        default:
            /*
                In this case, we can't skip past an unknown
                section type, since we don't know the record size.
            */
            return false;
    }

    skip_bytes *= ilda->h.number_of_records;

    return (fseek(ilda->f, skip_bytes, SEEK_CUR) == 0);
}


/*
    This function inits the parser context, and performs a
    quick scan of the ILDA file. It looks at each section header,
    and caches the number of frames per projector.

    This should only ever be called once per context lifetime.
*/
void scan_file(ILDA* ilda)
{

    //read all of the headers, and take notes
    while(true)
    {
        //pull out the next header
        int status = read_header(ilda);
        if(STATUS_IS_HALTING(status))
            break;

        //this section contains no records, halt parsing
        if(ilda->h.number_of_records == 0)
            break;

        //if the section carries point data, count it as a frame
        switch(ilda->h.format)
        {
            case 0:
            case 1:
            case 4:
            case 5:
                //increment the number of frames for this projector
                ilda->current_projector()->n_frames++;

            //ignore all other section types
        }

        skip_to_next_section(ilda);
    }
}


int ilda_read(ILDA* ilda, size_t pd, FrameList& frame_list)
{
    if(ilda == NULL)
        return LZR_FAILURE;

    //check that this file is open for reading
    if(!ilda->read)
        return LZR_FAILURE;

    int status;

    //seek to the beginning of the file
    fseek(ilda->f, 0, SEEK_SET); //seek to the beginning of the file

    frame_list.clear();

    //read all sections until the end is reached
    while(true)
    {
        status = read_section_for_projector(ilda, (uint8_t) pd, frame_list);

        if(STATUS_IS_HALTING(status))
            break;
    }

    //convert internal ILDA status to LZR
    switch(status)
    {
        case ILDA_WARN:  return LZR_WARNING;
        case ILDA_ERROR: return LZR_FAILURE;
        case ILDA_HALT:  return LZR_SUCCESS;
        default:         return LZR_SUCCESS;
    }
}


} // namespace lzr
