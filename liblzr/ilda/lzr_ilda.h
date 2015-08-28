
#include <lzr.h>



/******************************************************************************/
/*  Header Definition                                                         */
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


/******************************************************************************/
/*  Status Code Definition                                                    */
/******************************************************************************/

typedef struct {
    uint8_t last_point : 1;
    uint8_t blanked    : 1;
    uint8_t            : 6; // the rest of the byte is unused
} ilda_status;


/******************************************************************************/
/*  Color Index Definition                                                    */
/******************************************************************************/

typedef struct {
    uint8_t r;    
    uint8_t g;
    uint8_t b;
} ilda_color;


/******************************************************************************/
/*  2D Point Definitions                                                      */
/******************************************************************************/

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


/******************************************************************************/
/*  3D Point Definitions                                                      */
/******************************************************************************/

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
