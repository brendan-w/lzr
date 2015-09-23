
#include <string.h> //memset() zeroing out struct
#include <lzr.h>
#include "lzr_ilda.h"


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
    h.number_of_records = (uint16_t) f->n_points;
    h.projector_id      = (uint8_t)  pd;
    h.total_frames      = (uint16_t) GET_PROJECTOR_DATA(ilda, pd)->n_frames;
    h.frame_number      = (uint16_t) ilda->current_frame;

    //convert to big-endian
    htobe_header(&h);

    //write the header
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

    ilda_projector* proj = GET_PROJECTOR_DATA(ilda, pd);
    proj->n_frames = n_frames;

    for(size_t i = 0; i < n_frames; i++)
    {
        ilda->current_frame = i;
        int r = write_frame(ilda, (frames + i), pd);
        if(r != LZR_SUCCESS)
            return r;
    }

    return LZR_SUCCESS;
}
