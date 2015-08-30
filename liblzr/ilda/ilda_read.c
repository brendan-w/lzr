
#include <string.h> //strcmp() for "ILDA" header
#include <lzr.h>
#include "lzr_ilda.h"


//macros for reading parser state
#define FORMAT(ilda)            (ilda->h.format_code)
#define NUMBER_OF_RECORDS(ilda) (ilda->h.number_of_records)



//reads a single record, and error checks the size
static bool read_record(ilda_parser* ilda, void* buffer, size_t buffer_size)
{
    size_t r = fread(buffer, 1, buffer_size, ilda->f);

    //make sure we got everything...
    if(r != buffer_size)
    {
        perror("Encountered incomplete record");
        return false;
    }

    return true;
}

// -------------------- Format 0 --------------------
static bool read_3d_indexed(ilda_parser* ilda)
{
    for(size_t i = 0; i < NUMBER_OF_RECORDS(ilda); i++)
    {
        lzr_point lzr_p;
        ilda_point_3d_indexed p;

        if(!read_record(ilda, (void*) &p, sizeof(ilda_point_3d_indexed)))
            return false;

        if(p.status.last_point)
            break;

        endian_3d(&p);
        ilda_indexed_to_lzr(ilda, p, lzr_p);

        // printf("(%d, %d, %d)\n", p.x, p.y, p.z);
        printf("(%f, %f)\n", lzr_p.x, lzr_p.y);
    }

    return true;
}

// -------------------- Format 2 --------------------
static bool read_colors(ilda_parser* ilda)
{
    ilda_projector* proj = GET_CURRENT_PROJECTOR(ilda);
    free_projector_palette(proj);

    proj->n_colors = NUMBER_OF_RECORDS(ilda);
    proj->colors = (ilda_color*)calloc(sizeof(ilda_color), proj->n_colors);

    for(size_t i = 0; i < NUMBER_OF_RECORDS(ilda); i++)
    {
        //read one color record

    }

    return true;
}


static bool read_header(ilda_parser* ilda)
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
            perror("Encountered incomplete ILDA section header");

        return false;
    }

    //is it prefixed with "ILDA"?
    if(strcmp("ILDA", ilda->h.ilda) != 0)
    {
        perror("Section header did not contain \"ILDA\"");
        return false;
    }

    //correct for the big-endianness of the file
    endian_header(&(ilda->h));

    printf("records: %d\n", ilda->h.number_of_records);

    return true;
}


static bool read_section(ilda_parser* ilda)
{
    //pull out the next header
    if(!read_header(ilda)) return false;

    //this section contains no records, halt parsing
    if(NUMBER_OF_RECORDS(ilda) == 0)
        return false;

    //invoke to the corresponding parser for this section type
    switch(FORMAT(ilda))
    {
        case 0:
            if(!read_3d_indexed(ilda)) return false;
            break;
        case 1:
            break;
        case 2:
            if(!read_colors(ilda)) return false;
            break;
        case 4:
            break;
        case 5:
            break;
        default:
            break;//skip unknown section types
    }

    return true;
}


//Main function

int lzr_ilda_read(char* filename)
{
    //init a parser
    ilda_parser* ilda = (ilda_parser*) malloc(sizeof(ilda_parser));

    //wipe the projector data (color and frame arrays)
    for(size_t p = 0; p < MAX_PROJECTORS; p++)
    {
        ilda_projector* proj = GET_PROJECTOR(ilda, p);
        proj->colors = NULL;
        proj->frames = NULL;
        proj->n_colors = 0;
        proj->n_frames = 0;
    }

    ilda->f = fopen(filename, "rb");

    if(ilda->f == NULL)
    {
        perror("Failed to open file");
        return LZR_FAILURE;
    }

    //============================================

    //read each section in the ILDA file
    while(true) { if(!read_section(ilda)) break; }

    //============================================

    //destruct the parser
    for(size_t p = 0; p < MAX_PROJECTORS; p++)
    {
        ilda_projector* proj = GET_PROJECTOR(ilda, p);
        free_projector_palette(proj);
    }

    fclose(ilda->f);
    free(ilda);

    return LZR_SUCCESS;
}
