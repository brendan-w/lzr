
#include <string.h> //strcmp() for "ILDA" header
#include <lzr.h>
#include "lzr_ilda.h"



/*
 * Point conversion
 * These were done with macros to handle both 2D and 3D types
 *
 * These assume that all point types have `x` and `y` members
 * extra dimensions are outright ignored (3D is orthographically projected)
 */

//arguments: (ilda_parser*, ilda_point_2d_true | ilda_point_3d_true, lzr_point)
#define ilda_true_to_lzr(ilda, ilda_p, lzr_p) {     \
    (lzr_p).x = (double) (ilda_p).x / INT16_MAX;    \
    (lzr_p).y = (double) (ilda_p).y / INT16_MAX;    \
    (lzr_p).r = (ilda_p).r;                         \
    (lzr_p).g = (ilda_p).g;                         \
    (lzr_p).b = (ilda_p).b;                         \
    (lzr_p).i = UINT8_MAX;                          \
    if((ilda_p).status.blanked)                     \
        LZR_POINT_BLANK((lzr_p));                   \
}

//Arguments: (ilda_parser*, ilda_point_2d_indexed | ilda_point_3d_indexed, lzr_point)
#define ilda_indexed_to_lzr(ilda, ilda_p, lzr_p) {                 \
    (lzr_p).x = (double) (ilda_p).x / INT16_MAX;                   \
    (lzr_p).y = (double) (ilda_p).y / INT16_MAX;                   \
    ilda_color c = current_palette_get((ilda), (ilda_p).color);    \
    (lzr_p).r = c.r;                                               \
    (lzr_p).g = c.g;                                               \
    (lzr_p).b = c.b;                                               \
    (lzr_p).i = UINT8_MAX;                                         \
    if((ilda_p).status.blanked)                                    \
        LZR_POINT_BLANK((lzr_p));                                  \
}



//reads a single record, and error checks the size
static int read_record(ilda_parser* ilda, void* buffer, size_t buffer_size)
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
    This function will save the number_of_records (points) value from the
    current header, into the current_frame of the user's buffer.
    It will also perform error checking for point overflows.
*/
static int save_num_points(ilda_parser* ilda, lzr_frame* buffer, size_t* output_n_points)
{
    int status = ILDA_CONTINUE;
    size_t n = NUMBER_OF_RECORDS(ilda);

    if(n > LZR_FRAME_MAX_POINTS)
    {
        ilda->error = "Too many points for lzr_frame. Partial frame loaded.";
        n = LZR_FRAME_MAX_POINTS;
        status = ILDA_WARN;
    }

    //save the frame length to the user's buffer
    buffer[ilda->current_frame].n_points = n;

    return status;
}

/*
 * Section body readers
 * These functions iterate over their section's records
 *
 * Points are saved off to the given buffer.
 * Colors are placed in that projector's color palette array
 */

// -------------------- Format 0 --------------------
static int read_3d_indexed(ilda_parser* ilda, lzr_frame* buffer)
{
    size_t n_points;
    int status = save_num_points(ilda, buffer, &n_points);

    //if there's alreadya a problem, return early
    if(STATUS_IS_HALTING(status)) return status;

    //iterate over the records
    for(size_t i = 0; i < n_points; i++)
    {
        lzr_point lzr_p;
        ilda_point_3d_indexed p;

        int r = read_record(ilda, (void*) &p, sizeof(ilda_point_3d_indexed));
        if(STATUS_IS_HALTING(r)) return r;

        //convert the ILDA point to a lzr_point
        betoh_3d(&p);
        ilda_indexed_to_lzr(ilda, p, lzr_p);

        //save the new point to the user's buffer
        buffer[ilda->current_frame].points[i] = lzr_p;

        //if this was the last point, stop
        if(p.status.last_point)
            break;
    }

    return status;
}

// -------------------- Format 2 --------------------
static int read_colors(ilda_parser* ilda)
{
    current_palette_init(ilda, NUMBER_OF_RECORDS(ilda));

    for(size_t i = 0; i < NUMBER_OF_RECORDS(ilda); i++)
    {
        ilda_color c;

        //read one color record
        int r = read_record(ilda, (void*) &c, sizeof(ilda_color));
        if(STATUS_IS_HALTING(r)) return r;

        //store the new color
        current_palette_set(ilda, i, c);
    }

    return ILDA_CONTINUE;
}


/*
 * Header Reader
 */
static int read_header(ilda_parser* ilda)
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
static int read_section_for_projector(ilda_parser* ilda, uint8_t pd, lzr_frame* buffer)
{
    int status;

    //pull out the next header
    status = read_header(ilda);
    if(STATUS_IS_HALTING(status)) return status;

    //this section contains no records, halt parsing
    if(NUMBER_OF_RECORDS(ilda) == 0)
        return ILDA_HALT;

    //if this section isn't marked for projector we're looking for, skip it 
    if(PROJECTOR(ilda) != pd)
        return ILDA_CONTINUE;

    //invoke the corresponding parser for this section type
    switch(FORMAT(ilda))
    {
        case 0:
            status = read_3d_indexed(ilda, buffer); break;
        case 1:
            break;
        case 2:
            status = read_colors(ilda);             break;
        case 4:
            break;
        case 5:
            break;
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
    This function inits the parser context, and performs a
    quick scan of the ILDA file. It looks at each section header,
    and caches the number of frames per projector.

    This should only ever be called once per context lifetime.
*/
static void scan_file(ilda_parser* ilda)
{

    //read all of the headers, and take notes
    while(true)
    {
        //pull out the next header
        int status = read_header(ilda);
        if(STATUS_IS_HALTING(status))
            break;

        //this section contains no records, halt parsing
        if(NUMBER_OF_RECORDS(ilda) == 0)
            break;

        //if the section carries point data, count it as a frame
        switch(FORMAT(ilda))
        {
            case 0:
            case 1:
            case 4:
            case 5:
                //increment the number of frames for this projector
                ilda->projectors[PROJECTOR(ilda)].n_frames++;

            //ignore all other section types
        }

        skip_to_next_section(ilda);
    }
}


/******************************************************************************/
/*  Public Functions                                                          */
/******************************************************************************/


void* lzr_ilda_read(char* filename)
{
    //init a parser
    ilda_parser* ilda = malloc_parser();
    ilda->f = fopen(filename, "rb");

    if(ilda->f == NULL)
    {
        perror("Failed to open file for reading");
        free(ilda);
        return NULL;
    }

    //scan the file, populate the frame counts
    scan_file(ilda);

    return (void*) ilda;
}


int lzr_ilda_read_frames(void* f, size_t pd, lzr_frame* buffer)
{
    ilda_parser* ilda = (ilda_parser*) f;

    if(ilda == NULL)
    {
        return LZR_FAILURE;
    }

    //seek to the beginning of the file
    fseek(ilda->f, 0, SEEK_SET); //seek to the beginning of the file
    ilda->current_frame = 0; //zero out the frame counter
    int status;

    //read all sections until the end is reached
    while(true)
    {
        status = read_section_for_projector(ilda, (uint8_t) pd, buffer);

        if(STATUS_IS_HALTING(status))
            break;
    }

    switch(status)
    {
        case ILDA_WARN:  return LZR_WARNING;
        case ILDA_ERROR: return LZR_FAILURE;
        case ILDA_HALT:  return LZR_SUCCESS;
        default:         return LZR_SUCCESS;
    }
}
