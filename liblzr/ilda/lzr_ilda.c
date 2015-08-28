
#include <stdlib.h>
#include <string.h>
#include <lzr.h>
#include "lzr_ilda.h"


//macros for reading parser state
#define CURRENT_FORMAT(ilda) (ilda->h.format_code)


//helper function to safely free the old table
static void clear_color_table(ilda_parser* ilda)
{
    if(ilda->c != NULL) free(ilda->c);
}



static bool read_colors(ilda_parser* ilda)
{
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

    //invoke to the corresponding parser for this section type
    switch(CURRENT_FORMAT(ilda))
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

int read_ilda(char* filename)
{
    //init a parser
    ilda_parser* ilda = (ilda_parser*) malloc(sizeof(ilda_parser));
    ilda->f = fopen(filename, "rb");
    ilda->c = NULL;

    //read each section in the file
    while(true) { if(!read_section(ilda)) break; }

    //destruct the parser
    fclose(ilda->f);
    free(ilda);
}
