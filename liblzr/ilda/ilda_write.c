
#include <string.h> //memset() for "ILDA" header
#include <lzr.h>
#include "lzr_ilda.h"


static void write_frame(ilda_parser* ilda, lzr_frame* f)
{

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

int lzr_ilda_write_frame(lzr_ilda_file* f, size_t pd, lzr_frame* frame)
{
    ilda_parser* ilda = (ilda_parser*) f;

    //zero out a new header
    ilda_header h;
    memset(&h, 0, sizeof(ilda_header));

    return LZR_SUCCESS;
}

int lzr_ilda_write_frames(lzr_ilda_file* f, size_t pd, lzr_frame* frames, size_t n_frames)
{
    for(size_t i = 0; i < n_frames; i++)
    {
        int r = lzr_ilda_write_frame(f, pd, (frames + i));
        if(r != LZR_SUCCESS)
            return r;
    }

    return LZR_SUCCESS;
}

