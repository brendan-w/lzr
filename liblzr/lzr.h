

/******************************************************************************/
/*                                                                            */
/*  liblzr - a suite of common tools for handling laser graphics              */
/*                                                                            */
/*  Provides:                                                                 */
/*      point structures                                                      */
/*      frame structures                                                      */
/*      point transformations                                                 */
/*      frame transformations                                                 */
/*      frame interpolator                                                    */
/*      frame optimizer                                                       */
/*      ILDA file reader/writer                                               */
/*      ZeroMQ frame transport                                                */
/*                                                                            */
/******************************************************************************/



#ifndef LZR_TYPES_H
#define LZR_TYPES_H

#include <zmq.h>
#include <stdint.h>
#include <stdbool.h>
#include <vector>


/******************************************************************************/
/*  LZR Return Codes                                                          */
/******************************************************************************/

#define LZR_SUCCESS                 0
#define LZR_FAILURE                -1
#define LZR_WARNING                -2
#define LZR_ERROR_TOO_MANY_POINTS  -3
#define LZR_ERROR_TOO_MANY_FRAMES  -4
#define LZR_ERROR_INVALID_PROPERTY -5



/******************************************************************************/
/*  LZR Points                                                                */
/******************************************************************************/

//point limits
#define LZR_POINT_POSITION_MIN -1.0
#define LZR_POINT_POSITION_MAX 1.0
#define LZR_POINT_COLOR_MIN    0
#define LZR_POINT_COLOR_MAX    255


class Point
{
public:
    double x;  //Position X   [-1.0, 1.0]
    double y;  //Position Y   [-1.0, 1.0]
    uint8_t r; //Red          [0, 255]
    uint8_t g; //Green        [0, 255]
    uint8_t b; //Blue         [0, 255]
    uint8_t i; //Blanking     [0, 255]

    Point();
    Point(double x, double y);
    Point(double x, double y, uint8_t r, uint8_t g, uint8_t b, uint8_t i);

    void blank();
    void unblank();
    bool is_blanked();
    Point lerp_to(const Point& other, double t);
    double sq_distance_to(const Point& other);
    bool equal_position(const Point& other);
    bool equal_color(const Point& other);
    bool operator==(const Point& other);
    bool operator!=(const Point& other);
};


/******************************************************************************/
/*  LZR Frames                                                                */
/******************************************************************************/



class Frame : public std::vector<Point>
{
public:
    Frame();
    Frame(size_t n);

    Frame& operator+=(const Point& p);
    Frame& operator+=(const Frame& other);
    bool operator==(const Frame& other);

    //transforms
    int rotate(double x_axis, double y_axis, double theta);
    int translate(double x, double y);
    int scale(double x, double y);

    Point bounding_box_center();
    Point average_center();
};



/******************************************************************************/
/*  Frame Transforms                                                          */
/******************************************************************************/

#define LZR_BOUNDING_BOX 0
#define LZR_AVERAGE      1

/*
//rotates a frame around position specified by axis
int lzr_frame_rotate(lzr_frame* frame, lzr_point axis, double theta);

//translate all points within a frame
int lzr_frame_translate(lzr_frame* frame, lzr_point offset);

//scales all points within a frame
int lzr_frame_scale(lzr_frame* frame, double x, double y);

//centers the shape over the given point
int lzr_frame_move_to(lzr_frame* frame, lzr_point new_center, int method);

//appends frame B onto frame A with an optional blanking jump.
int lzr_frame_combine(lzr_frame* a, lzr_frame* b, bool blank);

//mirror the frame across the given Y-axis coordinate 
int lzr_frame_h_mirror(lzr_frame* frame, double y, bool blank);

//mirror the frame across the given Y-axis coordinate 
int lzr_frame_v_mirror(lzr_frame* frame, double x, bool blank);

//linearly duplicate the current frame
int lzr_frame_dup_linear(lzr_frame* frame, lzr_point offset, size_t n_dups, bool blank);

//radially duplicate the current frame
int lzr_frame_dup_radial(lzr_frame* frame, lzr_point axis, size_t n_dups, double angle, bool blank);

//clips a frame using the given mask. Points in the mask should define a closed
//polygon. All points outside the mask are discarded. Line segments that cross
//the mask boundry will have additional points inserted at that boundry.
int lzr_frame_mask(lzr_frame* frame, lzr_frame* mask);
*/

/******************************************************************************/
/*  LZR Interpolator                                                          */
/******************************************************************************/

/*
typedef void lzr_interpolator;

typedef enum {
    LZR_INTERP_MAX_DISTANCE
} interp_property;


//Allocates and returns a point to a new interpolator context.
lzr_interpolator* lzr_interpolator_create();

//Deallocator for the interpolator context
void lzr_interpolator_destroy(lzr_interpolator* interp);

//settings modifier
void lzr_interpolator_set(lzr_interpolator* interp, interp_property prop, unsigned long value);
#define lzr_interpolator_set(interp, prop, value) \
        lzr_interpolator_set(interp, prop, (unsigned long) (value))

//main interpolator function
int lzr_interpolator_run(lzr_interpolator* interp, lzr_frame* frame);
*/


/******************************************************************************/
/*  LZR Optimizer                                                             */
/******************************************************************************/

/*
typedef void lzr_optimizer;

typedef enum {
    LZR_OPT_ANCHOR_POINTS
} opt_property;


//Allocates and returns a point to a new optimizer context.
lzr_optimizer* lzr_optimizer_create();

//Deallocator for the optimizer context
void lzr_optimizer_destroy(lzr_optimizer* opt);

//settings modifier
void lzr_optimizer_set(lzr_optimizer* opt, opt_property prop, unsigned long value);
#define lzr_optimizer_set(opt, prop, value) \
        lzr_optimizer_set(opt, prop, (unsigned long) (value))

//main optimizer function.
int lzr_optimizer_run(lzr_optimizer* opt, lzr_frame* frame);
*/


/******************************************************************************/
/*  ILDA File Handlers                                                        */
/******************************************************************************/

/*
typedef void lzr_ilda_file;

//open ILDA file for reading or writing ----------------------------------------
//these functions will return NULL on failure

//opens the given ILDA file, and returns a parsing context
lzr_ilda_file* lzr_ilda_read(char* filename);

//opens or creates an ILDA file for writing, and returns a parsing context
lzr_ilda_file* lzr_ilda_write(char* filename);

//reading and writing functions ------------------------------------------------

//Reads all frames for the the given projector, and saves them
//in the given frame buffer. The frame buffer must be the size
//returned by `lzr_ilda_frame_count()`
int lzr_ilda_read_frames(lzr_ilda_file* f, size_t pd, lzr_frame* buffer);

//write frame(s) for the given projector to the ILDA file (file must be opened with lzr_ilda_write() )
int lzr_ilda_write_frames(lzr_ilda_file* f, size_t pd, lzr_frame* frames, size_t n_frames);

//returns the number of projectors that the ILDA specifies graphics for
size_t lzr_ilda_projector_count(lzr_ilda_file* f);

//returns the number of frames for a given projector
size_t lzr_ilda_frame_count(lzr_ilda_file* f, size_t pd);

//closes the ILDA file, and releases the parsing context
void lzr_ilda_close(lzr_ilda_file* f);
*/


/******************************************************************************/
/*  LZR ZeroMQ Facilities                                                     */
/******************************************************************************/

//create a ZMQ transmitter (publisher)
void* frame_pub_new(void* zmq_ctx, const char* address);

//create a ZMQ reciever (subscriber)
void* frame_sub_new(void* zmq_ctx, const char* address);

//send a single frame
int send_frame(void* pub, const Frame& frame);

//recieve a single frame (blocking)
int recv_frame(void* sub, Frame& frame);

//the default LZR endpoint
#ifndef LZR_ZMQ_ENDPOINT
# define LZR_ZMQ_ENDPOINT "tcp://127.0.0.1:5555"
#endif



#endif /* LZR_TYPES_H */
