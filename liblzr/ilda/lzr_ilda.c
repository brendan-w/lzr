
#include <lzr.h>
#include "lzr_ilda.h"


static bool read_header(FILE* f, ilda_header* h)
{
    //read one ILDA header
    size_t r = fread((void*) h, sizeof(ilda_header), 1, f);
    return (r == sizeof(ilda_header));
}

static bool read_section(FILE* f)
{
    ilda_header h;

    if(!read_header(f, &h))
        return false;

    switch(h.format_code)
    {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 4:
            break;
        case 5:
            break;
        default:
            return false;
    }

    return true;
}

int read_ilda(char* filename)
{
    FILE* f = fopen(filename, "rb");

    //read all sections
    bool more = true;
    while(more) { more = read_section(f); }
}
