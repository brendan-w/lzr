
#include <stdlib.h>
#include <string.h>
#include <lzr.h>
#include "lzr_ilda.h"


//macros for reading parser state
#define FORMAT(ilda)            (ilda->h.format_code)
#define NUMBER_OF_RECORDS(ilda) (ilda->h.number_of_records)


// Format 0
static bool read_3d_indexed(ilda_parser* ilda)
{

    for(size_t i = 0; i < NUMBER_OF_RECORDS(ilda); i++)
    {
        ilda_point_3d_indexed p;

        //read one point record
        size_t r = fread((void*) &p,
                         1,
                         sizeof(ilda_point_3d_indexed),
                         ilda->f);

        if(r != sizeof(ilda_point_3d_indexed))
        {
            perror("Encountered incomplete ILDA section header");
            return false;
        }

        endian_3d_indexed(&p);

        printf("(%d, %d, %d)\n", p.x, p.y, p.z);
    }

    return true;
}

// Format 2
static bool read_colors(ilda_parser* ilda)
{
    free_color_table(ilda);

    ilda->nc = NUMBER_OF_RECORDS(ilda);
    ilda->c = (ilda_color*)calloc(sizeof(ilda_color), ilda->nc);

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
    ilda->c = NULL;
    ilda->f = fopen(filename, "rb");

    if(ilda->f == NULL)
    {
        perror("Failed to open file");
        return LZR_FAILURE;
    }

    //read each section in the file
    while(true) { if(!read_section(ilda)) break; }

    //destruct the parser
    fclose(ilda->f);
    free_color_table(ilda);
    free(ilda);

    return LZR_SUCCESS;
}
