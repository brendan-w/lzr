

/******************************************************************************/
/*                                                                            */
/*  liblzr - a suite of common tools for handling laser graphics              */
/*                                                                            */
/*  Provides:                                                                 */
/*      point structures                                                      */
/*      frame structures                                                      */
/*      frame optimizer                                                       */
/*      frame interpolator                                                    */
/*      time definitions                                                      */
/*      ZeroMQ transport                                                      */
/*                                                                            */
/******************************************************************************/



#ifndef LZR_TYPES_H
#define LZR_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/******************************************************************************/
/*  LZR Points                                                                */
/******************************************************************************/

//point constants
#define LZR_POINT_MAX_POSITION 32768
#define LZR_POINT_MAX_COLOR    65535

typedef struct {
    int16_t x;  //position X
    int16_t y;  //position Y
    uint16_t r; //Red
    uint16_t g; //Green
    uint16_t b; //Blue
    uint16_t i; //Blanking
} lzr_point;

//point macros

//square of the distance between two points
#define LZR_POINT_SQ_DISTANCE(a, b) ( (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) )

//blanks the given point
#define LZR_POINT_BLANK(p)          p.r = p.g = p.b = p.i = 0

// returns boolean for whether the given point is blanked
#define LZR_POINT_IS_BLANKED(p)     ( (p.i == 0) || (p.r + p.g + p.b == 0) )

//are two points at the same position
#define LZR_POINTS_SAME_POS(a, b)   ( (a.x == b.x) && (a.y == b.y) )

//are two points the same color
#define LZR_POINTS_SAME_COLOR(a, b) ( (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.i == b.i) )

//check if two points are the same color, and at the same position
#define LZR_POINTS_EQUAL(a, b)      ( POINTS_SAME_POS(a, b) && POINTS_SAME_COLOR(a, b) )


/******************************************************************************/
/*  LZR Frames                                                                */
/******************************************************************************/

//frame constants
#define LZR_FRAME_MAX_POINTS 2000  // = 60,000 pps / 30 fps

typedef struct {
  lzr_point points[LZR_FRAME_MAX_POINTS];
  uint16_t n_points;
} lzr_frame;


/******************************************************************************/
/*  LZR Time                                                                  */
/******************************************************************************/

//sometimes this represents seconds, others, it represents normallized time
//Normalized time is ussually notated `nt` while time in seconds is simply `t`
typedef double lzr_time;


/******************************************************************************/
/*  LZR ZeroMQ Facilities                                                     */
/******************************************************************************/

//create a ZMQ transmitter (publisher)
void* lzr_create_frame_tx(void* zmq_ctx, char* address);

//create a ZMQ reciever (subscriber)
void* lzr_create_frame_rx(void* zmq_ctx, char* address);

//send a single frame
int lzr_send_frame(void* tx, lzr_frame* frame);

//recieve a single frame
int lzr_recv_frame(void* rx, lzr_frame* frame);


//the default LZR endpoint
#ifndef LZR_ZMQ_ENDPOINT
#define LZR_ZMQ_ENDPOINT "tcp://127.0.0.1:5555"
#endif


/******************************************************************************/
/*  LZR Optimizer                                                             */
/******************************************************************************/

typedef void lzr_optimizer;

typedef enum {
    LZR_OPT_MAX_POINTS,
    LZR_OPT_ANCHOR_POINTS,
} opt_property;


//Allocates and returns a point to a new optimizer context.
lzr_optimizer* lzr_create_optimizer(size_t max_points);

//Deallocator for the optimizer context
void lzr_destroy_optimizer(lzr_optimizer* opt);

/*
    Main optimizer function. Accepts an array of lzr_points as input,

    params:
           opt : the optimizer context
        points : pointer to an array of lzr_points
             n : length of the points array

    returns:
        The number of new points written to the array. Original data
        is overwritten.
*/
size_t lzr_optimize(lzr_optimizer* opt, lzr_point* points, size_t n);

//settings modifier
void lzr_optimizer_set(lzr_optimizer* _opt, opt_property prop, int value);

//settings getter
int lzr_optimizer_get(lzr_optimizer* _opt, opt_property prop);


/******************************************************************************/
/*  LZR Interpolator                                                          */
/******************************************************************************/

typedef void lzr_interpolator;

typedef enum {
    LZR_INTERP_MAX_DISTANCE,
} interp_property;


//Allocates and returns a point to a new interpolator context.
lzr_interpolator* lzr_interpolator_create();

//Deallocator for the interpolator context
void lzr_interpolator_destroy(lzr_interpolator* interp);

//settings modifier
void lzr_interpolator_set(lzr_interpolator* _opt, interp_property prop, int value);

//settings getter
int lzr_interpolator_get(lzr_interpolator* _opt, interp_property prop);

/*
    Main interpolator function.

    params:
        interp : the interpolator context
        frame  : the frame of points to be interpolated

    returns:
        0  : success
        -1 : error, not enough room for additional points
        -2 : error, LZR_INTERP_MAX_DISTANCE was set to zero
*/
int lzr_interpolator_run(lzr_interpolator* interp, lzr_frame* frame);



#ifdef __cplusplus
} // extern "c"
#endif

#endif /* LZR_TYPES_H */
