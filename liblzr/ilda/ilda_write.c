
#include <string.h> //memset() for "ILDA" header
#include <lzr.h>
#include "lzr_ilda.h"


static int write_frame(ilda_parser* ilda, lzr_frame* f, size_t pd)
{
    //zero out a new header
    ilda_header h;
    memset(&h, 0, sizeof(ilda_header));

    return LZR_SUCCESS;
}


/******************************************************************************/
/*  Public Functions                                                          */
/******************************************************************************/

lzr_ilda_file* lzr_ilda_write(char* filename)
{
    //init a parser
    ilda_parser* ilda = (ilda_parser*) malloc(sizeof(ilda_parser));
    ilda->f = fopen(filename, "wb");

    if(ilda->f == NULL)
    {
        perror("Failed to open file for writing");
        return NULL;
    }

    return (void*) ilda;

}

int lzr_ilda_write_frames(lzr_ilda_file* f, size_t pd, lzr_frame* frames, size_t n_frames)
{
    ilda_parser* ilda = (ilda_parser*) f;

    for(size_t i = 0; i < n_frames; i++)
    {
        int r = write_frame(ilda, (frames + i), pd);
        if(r != LZR_SUCCESS)
            return r;
    }

    return LZR_SUCCESS;
}
