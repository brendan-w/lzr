
#include <string.h> //memset() zeroing out struct
#include <lzr.h>
#include "lzr_ilda.h"



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


static int write_point(ilda_parser* ilda, lzr_point lzr_p)
{
    ilda_point_2d_true p;

    p.x = (int16_t) INT16_MAX * lzr_p.x;
    p.y = (int16_t) INT16_MAX * lzr_p.y;
    p.r = lzr_p.r;
    p.g = lzr_p.g;
    p.b = lzr_p.b;
    p.status.blanked = LZR_POINT_IS_BLANKED(lzr_p);

    //convert to big-endian
    htobe_2d(&p);
    fwrite((void*) &p, 1, sizeof(ilda_header), ilda->f);

    return LZR_SUCCESS;
}

static int write_frame(ilda_parser* ilda, lzr_frame* f, size_t pd)
{
    //zero out a new header
    ilda_header h;
    memset(&h, 0, sizeof(ilda_header));

    //set header details
    h.total_records = (uint16_t) f->n_points;

    //convert to big-endian
    htobe_header(&h);
    fwrite((void*) &h, 1, sizeof(ilda_header), ilda->f);

    //write each point to the file
    for(size_t i = 0; i < f->n_points; i++)
    {
        write_point(ilda, f->points[i]);
    }

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
