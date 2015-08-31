

#ifndef LZR_ILDA_H
#define LZR_ILDA_H

#include <stdlib.h>
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
    uint8_t            : 6; // the rest of the byte is unused
    uint8_t blanked    : 1;
    uint8_t last_point : 1;
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

#define MAX_PROJECTORS 256

typedef struct {
    // NOTE: We don't actually store the frames here,
    // they are copied directly to the user's array.
    // This struct is mostly for book-keeping
    size_t      n_frames; // the size of the frame array
    ilda_color* colors;   // the current color table
    size_t      n_colors; // number of colors in the palette
} ilda_projector;

typedef struct {
    FILE*          f;                          // the current file
    ilda_projector projectors[MAX_PROJECTORS]; // the per-projector data (colors and frames)

    //the following fields are only relevant during ONE
    //API call. Once the call is complete, these should be
    //considered invalid. They're only here for convenience.
    ilda_header    h;             // the current section header
    size_t         current_frame; // the current frame number being read
} ilda_parser;



/******************************************************************************/
/*  ILDA Color Utils                                                          */
/******************************************************************************/


// the ILDA default color table (defined in ilda_utils.c)
extern const ilda_color ilda_palette[];
extern const size_t ilda_color_count;

// solid color indices
#define ILDA_RED     0
#define ILDA_YELLOW  16
#define ILDA_GREEN   24
#define ILDA_CYAN    31
#define ILDA_BLUE    40
#define ILDA_MAGENTA 48
#define ILDA_WHITE   56


//malloc's a new color palette, freeing any previous one
void current_palette_init(ilda_parser* ilda, size_t n_colors);

//helper function to safely free the color palette for the given projector
void free_projector_palette(ilda_projector* p);

//safe color set for the current projector's palette
void current_palette_set(ilda_parser* ilda, size_t i, ilda_color c);

//safe color lookup for the current projector
//if a palette hasn't been defined, then the default ILDA palette is used
ilda_color current_palette_get(ilda_parser* ilda, size_t i);



/******************************************************************************/
/*  ILDA Utils                                                                */
/******************************************************************************/

//macros for reading parser state
#define FORMAT(ilda)            (ilda->h.format_code)
#define NUMBER_OF_RECORDS(ilda) (ilda->h.number_of_records)
#define PROJECTOR(ilda)         (ilda->h.projector_id)

//returns a pointer to the ilda_projector element
//arguments (ilda_parser*, uint8_t)
#define GET_PROJECTOR_DATA(ilda, i) ( (ilda)->projectors + (i) )

//same as GET_PROJECTOR_DATA, but returns the projector corresponding to
//`projector_id` in the header
//arguments (ilda_parser*)
#define GET_CURRENT_PROJECTOR_DATA(ilda) ( GET_PROJECTOR_DATA((ilda), (ilda)->h.projector_id ) )


/*
 * ILDA big-endian handlers
 * Will convert all big endian numbers to the hosts endianness
 *
 * These were done with macros to handle both indexed and true color types
 */

//arguments: (ilda_header*)
#define endian_header(header) {                                            \
    (header)->number_of_records = be16toh((header)->number_of_records);    \
    (header)->record_number     = be16toh((header)->record_number);        \
    (header)->total_records     = be16toh((header)->total_records);        \
}

//arguments: (ilda_point_2d_true* | ilda_point_2d_indexed*)
#define endian_2d(point) {                                    \
    (point)->x = (int16_t) be16toh((uint16_t) (point)->x);    \
    (point)->y = (int16_t) be16toh((uint16_t) (point)->y);    \
}

//arguments: (ilda_point_3d_true* | ilda_point_3d_indexed*)
#define endian_3d(point) {                                    \
    (point)->x = (int16_t) be16toh((uint16_t) (point)->x);    \
    (point)->y = (int16_t) be16toh((uint16_t) (point)->y);    \
    (point)->z = (int16_t) be16toh((uint16_t) (point)->z);    \
}


#endif /* LZR_ILDA_H */
