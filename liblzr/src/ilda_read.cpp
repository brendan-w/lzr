
#include <string.h> //strcmp() for "ILDA" header
#include <liblzr.hpp>
#include "ilda.hpp"

namespace lzr {


// function signiture for point readers
typedef int (*point_reader)(ILDA* ilda, Point& p);



//reads a single record, and error checks the size
static int read_record(ILDA* ilda, void* buffer, size_t buffer_size)
{
    if(ilda->f)
    {
        size_t r = fread(buffer, 1, buffer_size, ilda->f);

        //make sure we got everything...
        if(r != buffer_size)
        {
            ilda->error = "Encountered incomplete record";
            return ILDA_ERROR;
        }
    }
    else if(ilda->data)
    {
        // check if data is available for record
        if(ilda->data_index + buffer_size >= ilda->data_size)
        {
            ilda->error = "Encountered incomplete record";
            return ILDA_ERROR;
        }

        memcpy(buffer,
               (const void*) &(ilda->data[ilda->data_index]),
               buffer_size);
        ilda->data_index += buffer_size;
    }

    return ILDA_CONTINUE;
}


/*
 * Record readers/decoders
 * These functions translate ILDA structs into LZR structs
 *
 * Data from point records will be loaded into the given lzr::Point& p
 * Colors are placed in the corresponding projector's color palette array
 */

// ================================ Format 0 ================================
static int read_3d_indexed(ILDA* ilda, Point& p)
{
    static_assert(sizeof(ilda_point_3d_indexed) == 8, "3D Indexed point is not 8 bytes!");
    ilda_point_3d_indexed ilda_p;

    int r = read_record(ilda, (void*) &ilda_p, sizeof(ilda_point_3d_indexed));
    if(STATUS_IS_HALTING(r)) return r;

    //convert the ILDA point to a Point
    betoh_3d(&ilda_p);

    p.x = (float) ilda_p.x / INT16_MAX;
    p.y = (float) ilda_p.y / INT16_MAX;
    // ilda_p.z is completely discarded (orthographically projected)
    ilda_color c = ilda->current_projector()->lookup_color(ilda_p.color);
    p.r = c.r;
    p.g = c.g;
    p.b = c.b;
    p.i = UINT8_MAX;
    if(ilda_p.status.blanked)
        p.blank();

    return ILDA_CONTINUE;
}


// ================================ Format 1 ================================
static int read_2d_indexed(ILDA* ilda, Point& p)
{
    static_assert(sizeof(ilda_point_2d_indexed) == 6, "2D Indexed point is not 6 bytes!");
    ilda_point_2d_indexed ilda_p;

    int r = read_record(ilda, (void*) &ilda_p, sizeof(ilda_point_2d_indexed));
    if(STATUS_IS_HALTING(r)) return r;

    //convert the ILDA point to a Point
    betoh_2d(&ilda_p);

    p.x = (float) ilda_p.x / INT16_MAX;
    p.y = (float) ilda_p.y / INT16_MAX;
    ilda_color c = ilda->current_projector()->lookup_color(ilda_p.color);
    p.r = c.r;
    p.g = c.g;
    p.b = c.b;
    p.i = UINT8_MAX;
    if(ilda_p.status.blanked)
        p.blank();

    return ILDA_CONTINUE;
}


// ================================ Format 2 ================================
//NOTE: unlike the point readers, this function reads ALL of its records in one call
static int read_colors(ILDA* ilda)
{
    static_assert(sizeof(ilda_color) == 3, "Color point is not 3 bytes!");
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


// ================================ Format 4 ================================
static int read_3d_true(ILDA* ilda, Point& p)
{
    static_assert(sizeof(ilda_point_3d_true) == 10, "3D True-color point is not 10 bytes!");
    ilda_point_3d_true ilda_p;

    int r = read_record(ilda, (void*) &ilda_p, sizeof(ilda_point_3d_true));
    if(STATUS_IS_HALTING(r)) return r;

    //convert the ILDA point to a Point
    betoh_3d(&ilda_p);

    //read the point data
    p.x = (float) ilda_p.x / INT16_MAX;
    p.y = (float) ilda_p.y / INT16_MAX;
    // ilda_p.z is completely discarded (orthographically projected)
    p.r = ilda_p.r;
    p.g = ilda_p.g;
    p.b = ilda_p.b;
    p.i = UINT8_MAX;

    if(ilda_p.status.blanked)
        p.blank();

    return ILDA_CONTINUE;
}

// ================================ Format 5 ================================
static int read_2d_true(ILDA* ilda, Point& p)
{
    static_assert(sizeof(ilda_point_2d_true) == 8, "2D True-color point is not 8 bytes!");
    ilda_point_2d_true ilda_p;

    int r = read_record(ilda, (void*) &ilda_p, sizeof(ilda_point_2d_true));
    if(STATUS_IS_HALTING(r)) return r;

    //convert the ILDA point to a Point
    betoh_2d(&ilda_p);

    //read the point data
    p.x = (float) ilda_p.x / INT16_MAX;
    p.y = (float) ilda_p.y / INT16_MAX;
    p.r = ilda_p.r;
    p.g = ilda_p.g;
    p.b = ilda_p.b;
    p.i = UINT8_MAX;

    if(ilda_p.status.blanked)
        p.blank();

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

    //lookup the number of points
    size_t n = ilda->h.number_of_records;

    if(n == 0)
    {
        ilda->error = "Found frame with no points.";
        status = ILDA_WARN;
    }

    //allocate them all at once
    Frame frame(n);

    //iterate over the records
    for(size_t i = 0; i < n; i++)
    {
        Point point;
        int r = read_point(ilda, point);
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


//reads a single section (frame) of the ILDA file
//returns boolean for whether to continue parsing
static int read_section_for_projector(ILDA* ilda, uint8_t pd, FrameList& frame_list)
{
    int status;

    //pull out the next header
    status = read_header(ilda);
    if(STATUS_IS_HALTING(status))
        return status;

    //if this section isn't marked for projector we're looking for, skip it 
    if(ilda->h.projector_id != pd)
    {
        skip_to_next_section(ilda);
        return ILDA_CONTINUE;
    }

    //invoke the corresponding parser for this section type
    switch(ilda->h.format)
    {
        case FORMAT_0_3D_INDEXED:
            status = read_frame(ilda, frame_list, read_3d_indexed); break;
        case FORMAT_1_2D_INDEXED:
            status = read_frame(ilda, frame_list, read_2d_indexed); break;
        case FORMAT_2_PALETTE:
            status = read_colors(ilda); break;
        case FORMAT_4_3D_TRUE:
            status = read_frame(ilda, frame_list, read_3d_true); break;
        case FORMAT_5_2D_TRUE:
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


int ilda_read(ILDA* ilda, size_t pd, FrameList& frame_list)
{
    char name[9]; // dummy placeholders, since the user doesn't want them
    char company[9];
    return ilda_read(ilda, pd, frame_list, name, company);
}

int ilda_read(ILDA* ilda, size_t pd, FrameList& frame_list, char* name, char* company)
{
    if(ilda == NULL)
        return LZR_FAILURE;

    //check that this file is open for reading
    if(!ilda->read)
        return LZR_FAILURE;

    //seek to the beginning of the file
    int status = seek_to_start(ilda);

    if(STATUS_IS_HALTING(status))
        return LZR_FAILURE;

    frame_list.clear();

    //read all sections until the end is reached
    bool first = true;
    while(!STATUS_IS_HALTING(status))
    {
        status = read_section_for_projector(ilda, (uint8_t) pd, frame_list);

        if(first)
        {
            //copy the name/company strings from the first frame
            strncpy(name, ilda->h.name, sizeof(ilda->h.name));
            strncpy(company, ilda->h.company, sizeof(ilda->h.company));
            
            //NOTE: the ILDA format does not contain null terminators
            //while strncpy will null terminate if the string is less
            //than N bytes, a string of max length (8) will not be terminated
            //do this manually
            
            name[sizeof(ilda->h.name)] = '\0';
            company[sizeof(ilda->h.name)] = '\0';
            first = false;
        }
    }

    return ERROR_TO_LZR(status);
}


} // namespace lzr
