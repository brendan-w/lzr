
#include <stdlib.h>
#include <string.h>
#include <lzr.h>
#include "lzr_ilda.h"


//macros for reading parser state
#define FORMAT(ilda)            (ilda->h.format_code)
#define NUMBER_OF_RECORDS(ilda) (ilda->h.number_of_records)


//helper function to safely free any old color palette
static void free_color_table(ilda_parser* ilda)
{
    ilda->nc = 0;
    if(ilda->c != NULL) free(ilda->c);
}

//safe color lookup
//if a palette hasn't been defined, then the default ILDA palette is used
static ilda_color lookup_color(ilda_parser* ilda, size_t i)
{
    if(ilda->c == NULL)
    {
        //if no palette was defined, lookup in the default
        if(i < ilda_color_count) return ilda_palette[i];
        else                       return ilda_palette[ILDA_WHITE];
    }
    else
    {
        //use the custom palette
        if(i < ilda->nc) return ilda->c[i];
        else             return ilda->c[ILDA_WHITE];
    }
}



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
    //get a pointer to the header struct
    void* h = (void*) &(ilda->h);

    //read one ILDA header
    size_t r = fread(h, sizeof(ilda_header), 1, ilda->f);

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
    ilda->f = fopen(filename, "rb");
    ilda->c = NULL;

    //read each section in the file
    while(true) { if(!read_section(ilda)) break; }

    //destruct the parser
    fclose(ilda->f);
    free_color_table(ilda);
    free(ilda);

    return LZR_SUCCESS;
}
