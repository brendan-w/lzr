
#include <string.h> //strcmp()
#include <liblzr.h>
#include "ilda.h"

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
        scan_file(ilda);
    }
    else if(strcmp(mode, "w") == 0)
    {
        ilda->read = false;
        ilda->f = fopen(filename, "wb");

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


void ilda_close(ILDA* ilda)
{
    //if we were writing a file, we need to tack on a closing header
    if(!ilda->read)
        write_closer(ilda);

    fclose(ilda->f);
    delete ilda;
}


size_t ilda_frame_count(ILDA* ilda, size_t pd)
{
    if(pd > MAX_PROJECTORS)
        return 0;

    return ilda->projectors[pd].n_frames;
}


size_t ilda_projector_count(ILDA* ilda)
{
    size_t n = 0;

    for(size_t pd = 0; pd < MAX_PROJECTORS; pd++)
    {
        if(ilda_frame_count(ilda, pd) > 0)
            n++;
    }

    return n;
}



/******************************************************************************/
/*  Internal Utils                                                            */
/******************************************************************************/


ILDA::ILDA()
{
    f = NULL;

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


} // namespace lzr
