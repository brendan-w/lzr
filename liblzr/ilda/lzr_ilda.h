

#ifndef LZR_ILDA_H
#define LZR_ILDA_H

#include <stdio.h>
#include <endian.h>
#include <lzr.h>


/******************************************************************************/
/*  ILDA Structure Definitions                                                */
/******************************************************************************/

typedef struct {
    char     ilda[4];
    char     reserved_a[3];
    uint8_t  format_code;
    char     name[8];
    char     company_name[8];
    uint16_t number_of_records;
    uint16_t record_number;
    uint16_t total_records;
    uint8_t  projector_id;
    uint8_t  reserved_b;
} ilda_header;


typedef struct {
    uint8_t last_point : 1;
    uint8_t blanked    : 1;
    uint8_t            : 6; // the rest of the byte is unused
} ilda_status;


typedef struct {
    uint8_t r;    
    uint8_t g;
    uint8_t b;
} ilda_color;


typedef struct {
    int16_t     x;
    int16_t     y;
    uint8_t     color;
    ilda_status status;
} ilda_point_2d_indexed;


typedef struct {
    int16_t     x;
    int16_t     y;
    ilda_status status;
    uint8_t     b; //can't use an ilda_color, since the fields are in a different order
    uint8_t     g;
    uint8_t     r;
} ilda_point_2d_true;


typedef struct {
    int16_t     x;
    int16_t     y;
    int16_t     z;
    ilda_status status;
    uint8_t     color;
} ilda_point_3d_indexed;


typedef struct {
    int16_t     x;
    int16_t     y;
    int16_t     z;
    ilda_status status;
    uint8_t     b; //can't use an ilda_color, since the fields are in a different order
    uint8_t     g;
    uint8_t     r;
} ilda_point_3d_true;


/******************************************************************************/
/*  Parser Context                                                            */
/******************************************************************************/

typedef struct {
    FILE*       f;  // the current file
    ilda_header h;  // the current section header
    ilda_color* c;  // the current color table
    size_t      nc; // number of colors in the palette
    lzr_frame* frames;
} ilda_parser;


/******************************************************************************/
/*  ILDA Utils                                                                */
/******************************************************************************/

// the ILDA default color table (defined in ilda_utils.c)
extern const ilda_color ilda_palette[];
extern const int ilda_color_count;

// solid color indices
#define ILDA_RED     0
#define ILDA_YELLOW  16
#define ILDA_GREEN   24
#define ILDA_CYAN    31
#define ILDA_BLUE    40
#define ILDA_MAGENTA 48
#define ILDA_WHITE   56

//helper function to safely free any old color palette
void free_color_table(ilda_parser* ilda);

//safe color lookup
//if a palette hasn't been defined, then the default ILDA palette is used
ilda_color lookup_color(ilda_parser* ilda, size_t i);

//endian handlers
void endian_header(ilda_header* h);
void endian_2d_indexed(ilda_point_2d_indexed* p);
void endian_2d_true(ilda_point_2d_true* p);
void endian_3d_indexed(ilda_point_3d_indexed* p);
void endian_3d_true(ilda_point_3d_true* p);


#endif /* LZR_ILDA_H */
