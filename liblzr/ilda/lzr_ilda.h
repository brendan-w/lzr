
#include <stdio.h>
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


// the ILDA default color table
ilda_color ilda_palette[] = {
//    R    G    B
    { 255, 0,   0   }, // Red
    { 255, 16,  0   },
    { 255, 32,  0   },
    { 255, 48,  0   },
    { 255, 64,  0   },
    { 255, 80,  0   },
    { 255, 96,  0   },
    { 255, 112, 0   },
    { 255, 128, 0   },
    { 255, 144, 0   },
    { 255, 160, 0   },
    { 255, 176, 0   },
    { 255, 192, 0   },
    { 255, 208, 0   },
    { 255, 224, 0   },
    { 255, 240, 0   },

    { 255, 255, 0   }, // Yellow
    { 224, 255, 0   },
    { 192, 255, 0   },
    { 160, 255, 0   },
    { 128, 255, 0   },
    { 96,  255, 0   },
    { 64,  255, 0   },
    { 32,  255, 0   },
    { 0,   255, 0   }, // Green
    { 0,   255, 36  },
    { 0,   255, 73  },
    { 0,   255, 109 },
    { 0,   255, 146 },
    { 0,   255, 182 },
    { 0,   255, 219 },
    { 0,   255, 255 }, // Cyan

    { 0,   227, 255 }, 
    { 0,   198, 255 },
    { 0,   170, 255 },
    { 0,   142, 255 },
    { 0,   113, 255 },
    { 0,   85,  255 },
    { 0,   56,  255 },
    { 0,   28,  255 },
    { 0,   0,   255 }, // Blue
    { 32,  0,   255 },
    { 64,  0,   255 },
    { 96,  0,   255 },
    { 128, 0,   255 },
    { 160, 0,   255 },
    { 192, 0,   255 },
    { 224, 0,   255 },

    { 255, 0,   255 }, // Magenta
    { 255, 32,  255 },
    { 255, 64,  255 },
    { 255, 96,  255 },
    { 255, 128, 255 },
    { 255, 160, 255 },
    { 255, 192, 255 },
    { 255, 224, 255 },
    { 255, 255, 255 }, // White
    { 255, 224, 224 },
    { 255, 192, 192 },
    { 255, 160, 160 },
    { 255, 128, 128 },
    { 255, 96,  96  },
    { 255, 64,  64  },
    { 255, 32,  32  }
};

#define ILDA_PALETTE_COUNT (sizeof(ilda_palette) / sizeof(ilda_color))

// solid color indices
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

typedef struct {
    FILE*       f;  // the current file
    ilda_header h;  // the current section header
    ilda_color* c;  // the current color table
    size_t      nc; // number of colors in the palette
    lzr_frame* frames;
} ilda_parser;
