

#ifndef LZR_ILDA_H
#define LZR_ILDA_H

#include <stdlib.h>
#include <stdio.h>
#include <endian.h>
#include <lzr.h>


/******************************************************************************/
/*  Internal Return Codes                                                     */
/******************************************************************************/

#define ILDA_CONTINUE  1 //all is well
#define ILDA_HALT      0 //a peaceful end of parsing
#define ILDA_WARN     -1 //non-fatal problem
#define ILDA_ERROR    -2 //fatal problem, halt parsing

#define STATUS_IS_HALTING(s) (( s == ILDA_HALT ) || (s == ILDA_ERROR))



/******************************************************************************/
/*  ILDA Structure Definitions                                                */
/******************************************************************************/

typedef struct {
    char     ilda[4];
    char     reserved_a[3];
    uint8_t  format_code;
    char     name[8];
    char     company_name[8];
    uint16_t number_of_records; //number of points of color entries
    uint16_t frame_number; //also denotes color pallete number
    uint16_t total_frames;
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

#define MAX_PROJECTORS 256

class ILDA_Projector
{
    public:
        ilda_color lookup_color(size_t i);
        // NOTE: We don't actually store the frames here,
        // they are copied directly to the user's array.
        // This is mostly for book-keeping
        size_t n_frames; // the size of the frame array
        std::vector<ilda_color> palette;
};

// parsing context
class ILDA
{
    public:
        FILE* f;     // the current file
        bool read;   // if false, we're in write mode
        char* error; // error string

        //the following fields are only relevant during ONE
        //API call. Once the call is complete, these should be
        //considered invalid. They're only here for convenience.
        ilda_header h;        // the current section header
        size_t current_frame; // the current frame number being read

        ILDA_Projector* current_projector();

    private:
        ILDA_Projector projectors[MAX_PROJECTORS]; // the per-projector data (colors and frames)
};


/******************************************************************************/
/*  Forward declares for the parsing modules                                  */
/******************************************************************************/

void scan_file(ILDA* ilda);

//skips from the end of the current header,
//to the start of the next header
bool skip_to_next_section(ILDA* ilda);

//initialize a new parser context
//the opposite function would be `lzr_ilda_close()`
ILDA* malloc_parser();


/******************************************************************************/
/*  ILDA Utils                                                                */
/******************************************************************************/

//macros for reading parser state
#define FORMAT(ilda)            (ilda->h.format_code)
#define NUMBER_OF_RECORDS(ilda) (ilda->h.number_of_records)
#define PROJECTOR(ilda)         (ilda->h.projector_id)

//returns a pointer to the ilda_projector element
//arguments (ILDA*, uint8_t)
#define GET_PROJECTOR_DATA(ilda, i) ( (ilda)->projectors + (i) )

//same as GET_PROJECTOR_DATA, but returns the projector corresponding to
//`projector_id` in the header
//arguments (ILDA*)
#define GET_CURRENT_PROJECTOR_DATA(ilda) ( GET_PROJECTOR_DATA((ilda), (ilda)->h.projector_id ) )


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

#endif /* LZR_ILDA_H */
