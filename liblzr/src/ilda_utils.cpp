
#include <lzr.h>
#include "ilda_utils.h"


/******************************************************************************/
/*  ILDA Utils                                                                */
/******************************************************************************/


bool skip_to_next_section(ILDA* ilda)
{
    int skip_bytes = 0;

    switch(FORMAT(ilda))
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

    skip_bytes *= NUMBER_OF_RECORDS(ilda);

    return (fseek(ilda->f, skip_bytes, SEEK_CUR) == 0);
}

ILDA* malloc_parser()
{
    ILDA* ilda = (ILDA*) malloc(sizeof(ILDA));

    ilda->f = NULL;

    //wipe the projector data (color and frame arrays)
    for(size_t pd = 0; pd < MAX_PROJECTORS; pd++)
    {
        ilda_projector* proj = GET_PROJECTOR_DATA(ilda, pd);
        proj->colors = NULL;
        proj->n_colors = 0;
        proj->n_frames = 0;
    }

    return ilda;
}

/******************************************************************************/
/*  Public Functions                                                          */
/******************************************************************************/

void lzr_ilda_close(void* f)
{
    ILDA* ilda = (ILDA*) f;
    
    //destruct the parser
    for(size_t pd = 0; pd < MAX_PROJECTORS; pd++)
    {
        ilda_projector* proj = GET_PROJECTOR_DATA(ilda, pd);
        free_projector_palette(proj);
    }

    fclose(ilda->f);
    free(ilda);
}


size_t lzr_ilda_projector_count(void* f)
{
    ILDA* ilda = (ILDA*) f;

    size_t n = 0;
    for(size_t pd = 0; pd < MAX_PROJECTORS; pd++)
    {
        ilda_projector* proj = GET_PROJECTOR_DATA(ilda, pd);
        if(proj->n_frames > 0)
            n++;
    }

    return n;
}


size_t lzr_ilda_frame_count(void* f, size_t pd)
{
    ILDA* ilda = (ILDA*) f;

    if(pd > MAX_PROJECTORS)
        return 0;

    ilda_projector* proj = GET_PROJECTOR_DATA(ilda, pd);
    return (size_t) proj->n_frames;
}
