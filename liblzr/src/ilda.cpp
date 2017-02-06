
#include <string.h> //strcmp()
#include <liblzr.hpp>
#include "ilda.hpp"

namespace lzr {


/******************************************************************************/
/*  API Functions                                                             */
/******************************************************************************/

ILDA* ilda_open(const char* filename, const char* mode)
{
    ILDA* ilda = new ILDA();

    if(strcmp(mode, "r") == 0)
    {
        ilda->read = true;
        ilda->f = fopen(filename, "rb");

        if(ilda->f == NULL)
        {
            perror("Failed to open file for reading");
            delete ilda;
            return NULL;
        }

        //scan the file, populate the frame counts
        if(init_frame_counts(ilda) != ILDA_CONTINUE)
        {
            perror("Failed to initialize frame counts");
            delete ilda;
            return NULL;
        }
    }
    else if(strcmp(mode, "w") == 0)
    {
        ilda->read = false;
        ilda->f = fopen(filename, "wb+");

        if(ilda->f == NULL)
        {
            perror("Failed to open file for writing");
            delete ilda;
            return NULL;
        }
    }
    else
    {
        perror("Invalid ILDA file mode");
        //invalid mode
        delete ilda;
        return NULL;
    }

    return ilda;
}


int ilda_close(ILDA* ilda)
{
    if(ilda == NULL)
        return LZR_ERROR_INVALID_ARG;

    //if we were writing a file, we need to tack on a closing header
    int status = LZR_SUCCESS;

    if(!ilda->read)
        status = write_finish(ilda);

    fclose(ilda->f);
    delete ilda;

    return ERROR_TO_LZR(status);
}


size_t ilda_frame_count(ILDA* ilda, size_t pd)
{
    if(ilda == NULL)
        return LZR_ERROR_INVALID_ARG;

    if(pd > MAX_PROJECTORS)
    {
        ilda->error = "Projector descriptor outside of 0-255 range";
        return LZR_ERROR_INVALID_ARG;
    }

    return ilda->projectors[pd].n_frames;
}


size_t ilda_projector_count(ILDA* ilda)
{
    if(ilda == NULL)
        return LZR_ERROR_INVALID_ARG;

    size_t n = 0;

    for(size_t pd = 0; pd < MAX_PROJECTORS; pd++)
    {
        if(ilda_frame_count(ilda, pd) > 0)
            n++;
    }

    return n;
}

const char* ilda_error(ILDA* ilda)
{
    return ilda->error;
}


/******************************************************************************/
/*  Parser Context                                                            */
/******************************************************************************/


ILDA::ILDA()
  : f(nullptr)
  , error("")
{
    //wipe the projector data (color and frame arrays)
    for(size_t pd = 0; pd < MAX_PROJECTORS; pd++)
    {
        projectors[pd].n_frames = 0;
    }
}


ILDA_Projector* ILDA::current_projector()
{
    return (projectors + h.projector_id);
}


ilda_color ILDA_Projector::lookup_color(size_t i)
{
    if(palette.size() > 0)
    {
        //use the custom palette
        if(i < palette.size())
            return palette[i];
        else
            return ilda_palette[ILDA_WHITE];
    }
    else
    {
        //if no palette was defined, lookup in the default
        if(i < ilda_palette_size)
            return ilda_palette[i];
        else
            return ilda_palette[ILDA_WHITE];
    }
}

void ILDA_Projector::add_color_to_palette(ilda_color c)
{
    if(palette.size() < 256)
        palette.push_back(c);
    // else
        //TODO: handle this error
}

void ILDA_Projector::clear_palette()
{
    palette.clear();
}


/******************************************************************************/
/*  ILDA Utils                                                                */
/******************************************************************************/

int read_header(ILDA* ilda)
{
    static_assert(sizeof(ilda_header) == 32, "ILDA header is not 32 bytes!");

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
    if(strcmp(ILDA_MAGIC, ilda->h.ilda) != 0)
    {
        ilda->error = "Section header did not contain \"ILDA\"";
        return ILDA_ERROR;
    }

    //correct for the big-endianness of the file
    betoh_header(&(ilda->h));

    //this section contains no records, halt parsing
    if(ilda->h.number_of_records == 0)
        return ILDA_HALT;

    return ILDA_CONTINUE;
}

int skip_to_next_section(ILDA* ilda)
{
    int skip_bytes = 0;

    switch(ilda->h.format)
    {
        case FORMAT_0_3D_INDEXED: skip_bytes = sizeof(ilda_point_3d_indexed); break;
        case FORMAT_1_2D_INDEXED: skip_bytes = sizeof(ilda_point_2d_indexed); break;
        case FORMAT_2_PALETTE:    skip_bytes = sizeof(ilda_color);            break;
        case FORMAT_4_3D_TRUE:    skip_bytes = sizeof(ilda_point_3d_true);    break;
        case FORMAT_5_2D_TRUE:    skip_bytes = sizeof(ilda_point_2d_true);    break;
        default:
            /*
                In this case, we can't skip past an unknown
                section type, since we don't know the record size.
            */
            ilda->error = "Encountered unknown format type";
            return ILDA_ERROR;
    }

    skip_bytes *= ilda->h.number_of_records;

    if(fseek(ilda->f, skip_bytes, SEEK_CUR) != 0)
    {
        ilda->error = "Failed to skip to next section";
        return ILDA_ERROR;
    }

    return ILDA_CONTINUE;
}

int seek_to_start(ILDA* ilda)
{
    //seek to the start of the file
    if(fseek(ilda->f, 0, SEEK_SET) != 0)
    {
        ilda->error = "Failed to seek to start of file";
        return ILDA_ERROR;
    }

    return ILDA_CONTINUE;
}

int init_frame_counts(ILDA* ilda)
{
    int status = seek_to_start(ilda);

    if(STATUS_IS_HALTING(status))
        return status;

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
            case FORMAT_0_3D_INDEXED:
            case FORMAT_1_2D_INDEXED:
            case FORMAT_4_3D_TRUE:
            case FORMAT_5_2D_TRUE:
                //increment the number of frames for this projector
                ilda->current_projector()->n_frames++;

            //ignore all other section types
            //errors for other frame types are handled in skip_to_next_section()
        }

        skip_to_next_section(ilda);
    }

    return ILDA_CONTINUE;
}

} // namespace lzr
