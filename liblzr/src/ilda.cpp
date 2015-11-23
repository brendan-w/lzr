
#include <string.h> //strcmp()
#include "ilda.h"
#include <lzr.h>


/*
 * Main API functions
 */

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
    if(strcmp(mode, "w") == 0)
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
        //invalid mode
        delete ilda;
        return NULL;
    }

    return ilda;
}

void ilda_close(ILDA* ilda)
{
    delete ilda;
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
