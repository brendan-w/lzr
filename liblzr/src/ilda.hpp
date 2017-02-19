

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <liblzr.hpp>

#include "include/portable_endian.h"

namespace lzr {


/******************************************************************************/
/*  Internal Return Codes                                                     */
/******************************************************************************/

#define ILDA_HALT      1 //a peaceful end of parsing
#define ILDA_CONTINUE  LZR_SUCCESS //all is well
#define ILDA_WARN      LZR_WARNING //non-fatal problem
#define ILDA_ERROR     LZR_FAILURE //fatal problem, halt parsing

#define STATUS_IS_HALTING(s) (( s == ILDA_HALT ) || (s == ILDA_ERROR))

// simple, because we've defined most of them in terms of the LZR codes
#define ERROR_TO_LZR(e) ((e == ILDA_HALT) ? LZR_SUCCESS : e)


/******************************************************************************/
/*  ILDA Structure Definitions                                                */
/******************************************************************************/

#ifdef _MSC_VER
#define PACKED
#pragma pack(push,1)
#else
#define PACKED __attribute__ ((__packed__))
#endif

typedef struct PACKED {
    char     ilda[4];
    char     reserved_a[3];
    uint8_t  format;
    char     name[8];
    char     company[8];
    uint16_t number_of_records; //number of points of color entries
    uint16_t frame_number; //also denotes color pallete number
    uint16_t total_frames;
    uint8_t  projector_id;
    uint8_t  reserved_b;
} ilda_header;


typedef struct PACKED {
    uint8_t            : 6; // the rest of the byte is unused
    uint8_t blanked    : 1;
    uint8_t last_point : 1;
} ilda_status;


typedef struct PACKED {
    uint8_t r;    
    uint8_t g;
    uint8_t b;
} ilda_color;


typedef struct PACKED {
    int16_t     x;
    int16_t     y;
    ilda_status status;
    uint8_t     color;
} ilda_point_2d_indexed;


typedef struct PACKED {
    int16_t     x;
    int16_t     y;
    ilda_status status;
    uint8_t     b; //can't use an ilda_color, since the fields are in a different order
    uint8_t     g;
    uint8_t     r;
} ilda_point_2d_true;


typedef struct PACKED {
    int16_t     x;
    int16_t     y;
    int16_t     z;
    ilda_status status;
    uint8_t     color;
} ilda_point_3d_indexed;


typedef struct PACKED {
    int16_t     x;
    int16_t     y;
    int16_t     z;
    ilda_status status;
    uint8_t     b; //can't use an ilda_color, since the fields are in a different order
    uint8_t     g;
    uint8_t     r;
} ilda_point_3d_true;

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#undef PACKED


/******************************************************************************/
/*  ILDA Constants                                                            */
/******************************************************************************/

// file format

#define ILDA_MAGIC "ILDA"
#define FORMAT_0_3D_INDEXED 0
#define FORMAT_1_2D_INDEXED 1
#define FORMAT_2_PALETTE    2
#define FORMAT_4_3D_TRUE    4
#define FORMAT_5_2D_TRUE    5

#define MAX_PROJECTORS 256

// colors

extern const ilda_color ilda_palette[];
extern const size_t ilda_palette_size;

#define ILDA_RED     0
#define ILDA_YELLOW  16
#define ILDA_GREEN   24
#define ILDA_CYAN    31
#define ILDA_BLUE    40
#define ILDA_MAGENTA 48
#define ILDA_WHITE   56



/******************************************************************************/
/*  Parser Context                                                            */
/******************************************************************************/

 
class ILDA_Projector
{
    public:
        ilda_color lookup_color(size_t i);
        void add_color_to_palette(ilda_color c);
        void clear_palette();
        // NOTE: We don't actually store the frames here,
        // they are copied directly to the user's array.
        // This is mostly for book-keeping
        size_t n_frames; // the size of the frame array
    private:
        std::vector<ilda_color> palette;
};


// parsing context
class ILDA
{
    public:
        ILDA();

        FILE* f;     // the current file
        bool read;   // if false, we're in write mode
        const char* error; // error string

        ILDA_Projector* current_projector();
        ILDA_Projector projectors[MAX_PROJECTORS]; // the per-projector data (colors and frames)

        //the following fields are only relevant during ONE
        //API call. Once the call is complete, these should be
        //considered invalid. They're only here for convenience.
        ilda_header h; // the current section header
};



/******************************************************************************/
/*  ILDA Utils                                                                */
/******************************************************************************/


// reads a header from the file
int read_header(ILDA* ilda);

// seeks to the beginning of a file
int seek_to_start(ILDA* ilda);

/*
    This function inits the parser context, and performs a
    quick scan of the ILDA file. It looks at each section header,
    and caches the number of frames per projector.

    This should only ever be called once per context lifetime.
*/
int init_frame_counts(ILDA* ilda);

/*
    Call this AFTER reading a header. If you decide you aren't interested
    in the data within that frame, call this to skip to the next header.
*/
int skip_to_next_section(ILDA* ilda);

//writes the closing section header (where number_of_records is zero)
//this is only applicable to files opened for writing
int write_finish(ILDA* ilda);


/*
 * ILDA big-endian handlers
 * Will convert all big endian numbers to the hosts endianness
 *
 * These were done with macros to handle both indexed and true color types
 */

/*
 * From Big-Endian to Host
 */

//arguments: (ilda_header*)
#define betoh_header(header) {                                             \
    (header)->number_of_records = be16toh((header)->number_of_records);    \
    (header)->frame_number      = be16toh((header)->frame_number);         \
    (header)->total_frames      = be16toh((header)->total_frames);         \
}

//arguments: (ilda_point_2d_true* | ilda_point_2d_indexed*)
#define betoh_2d(point) {                                     \
    (point)->x = (int16_t) be16toh((uint16_t) (point)->x);    \
    (point)->y = (int16_t) be16toh((uint16_t) (point)->y);    \
}

//arguments: (ilda_point_3d_true* | ilda_point_3d_indexed*)
#define betoh_3d(point) {                                     \
    (point)->x = (int16_t) be16toh((uint16_t) (point)->x);    \
    (point)->y = (int16_t) be16toh((uint16_t) (point)->y);    \
    (point)->z = (int16_t) be16toh((uint16_t) (point)->z);    \
}


/*
 * From Host to Big-Endian
 */

#define htobe_header(header) {                                             \
    (header)->number_of_records = htobe16((header)->number_of_records);    \
    (header)->frame_number      = htobe16((header)->frame_number);         \
    (header)->total_frames      = htobe16((header)->total_frames);         \
}

#define htobe_2d(point) {                                     \
    (point)->x = (int16_t) htobe16((uint16_t) (point)->x);    \
    (point)->y = (int16_t) htobe16((uint16_t) (point)->y);    \
}

#define htobe_3d(point) {                                     \
    (point)->x = (int16_t) htobe16((uint16_t) (point)->x);    \
    (point)->y = (int16_t) htobe16((uint16_t) (point)->y);    \
    (point)->z = (int16_t) htobe16((uint16_t) (point)->z);    \
}


} // namespace lzr
