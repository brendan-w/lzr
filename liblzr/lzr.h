

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



#pragma once

#define LZR_VERSION "0.0.1"

#include <zmq.h>
#include <stdint.h>
#include <stdbool.h>
#include <vector>

namespace lzr {



/******************************************************************************/
/*  LZR Return Codes                                                          */
/******************************************************************************/

#define LZR_SUCCESS                 0
#define LZR_FAILURE                -1
#define LZR_WARNING                -2
#define LZR_ERROR_TOO_MANY_POINTS  -3
#define LZR_ERROR_TOO_MANY_FRAMES  -4
#define LZR_ERROR_INVALID_ARG      -5



/******************************************************************************/
/*  LZR Points                                                                */
/******************************************************************************/

//point limits
#define LZR_POSITION_MIN -1.0
#define LZR_POSITION_MAX 1.0
#define LZR_COLOR_MIN    0
#define LZR_COLOR_MAX    255


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
    bool is_lit();
    Point lerp_to(const Point& other, double t);
    double sq_distance_to(const Point& other);
    bool same_position_as(const Point& other) const;
    bool same_color_as(const Point& other) const;
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
};



/******************************************************************************/
/*  LZR Frames                                                                */
/******************************************************************************/

//OK as long as the Frame class doesn't save any state, which, it shouldn't...
//this is only a convenient way to attach functions to the type
class Frame : public std::vector<Point>
{
public:
    Frame();
    Frame(size_t n);

    Frame& add(const Point& p);
    Frame& add(const Frame& other);
    Frame& add_with_blank_jump(const Point& p);
    Frame& add_with_blank_jump(const Frame& other);

    Point bounding_box_center();
    Point average_center();

private:
    Frame& add_blank_jump_to(const Point& p);
    Frame& add_blank_jump_to(const Frame& other);
};



/******************************************************************************/
/*  LZR Frame Transforms                                                      */
/******************************************************************************/

int translate(Frame& frame, double x, double y);
int rotate(Frame& frame, Point center, double theta);
int scale(Frame& frame, Point center, double x, double y);
int mirror(Frame& frame, Point center, bool x, bool y);
int dup_mirror(Frame& frame, Point center, bool x, bool y, bool blank=true);
int dup_linear(Frame& frame, Point offset, size_t n_dups, bool blank=true);
int dup_radial(Frame& frame, Point center, size_t n_dups, double angle, bool blank=true);


//clips a frame using the given mask. Points in the mask should define a closed
//polygon. All points outside the mask are discarded. Line segments that cross
//the mask boundry will have additional points inserted at that boundry.
int mask(Frame& frame, const Frame& mask, bool inverse=false);



/******************************************************************************/
/*  LZR Animations                                                            */
/******************************************************************************/

typedef std::vector<Frame> FrameList;



/******************************************************************************/
/*  LZR Interpolator                                                          */
/******************************************************************************/

//type for interpolation functions
typedef double (*interpolation_func)(double t);

//interpolation functions
double linear(double t); /*----*----*----*----*----*----*----*----*/
double quad(double t);   /*---*---*-----*-----*-----*-----*---*---*/
double quart(double t);  /*-*---*-----*-------*-------*-----*---*-*/


//100 points from one side of the frame to the other
#define INTERP_DEFAULT ((LZR_POSITION_MAX - LZR_POSITION_MIN) / 100.0)

//main interpolator function
int interpolate(Frame& frame,
                double max_distance=INTERP_DEFAULT,
                interpolation_func func=linear);



/******************************************************************************/
/*  LZR Optimizer                                                             */
/******************************************************************************/

//20 points from one side of the frame to the other
#define BLANK_INTERP_DEFAULT ((LZR_POSITION_MAX - LZR_POSITION_MIN) / 5.0)

//forward declaration for optimizer internals
class Optimizer_Internals;

class Optimizer
{
public:
    Optimizer();
    ~Optimizer();
    int run(Frame& frame);

    //settings
    double path_split_angle       = 45;   //minimum angle (degrees) at which to consider lines to be seperate paths
    bool   reorder_paths          = true; //allow the optimizer to the change the order in which points are scanned
    size_t anchor_points_lit      = 1;    //minimum number of lit points to place at the start & end of line segments
    size_t anchor_points_blanked  = 2;    //minimum number of blanked points to place at the start & end of a line segment
    double blank_max_distance     = BLANK_INTERP_DEFAULT; //max distance for interpolation of blanking jumps
    interpolation_func blank_func = linear; //interpolation function to use for blanking jumps

private:
    Optimizer_Internals* internal;
};



/******************************************************************************/
/*  ILDA File Handlers                                                        */
/******************************************************************************/

class ILDA;

// "r" = read
// "w" = write
//Will return NULL on failure
ILDA* ilda_open(const char* filename, const char* mode);

//closes the ILDA file, and releases the parsing context
void ilda_close(ILDA* f);

//Reads all frames for the the given projector, and returns them
int ilda_read(ILDA* f, size_t pd, FrameList& frame_list);

//write frame(s) for the given projector to the ILDA file (file must be opened with lzr_ilda_write() )
int ilda_write(ILDA* f, size_t pd, FrameList& frame_list);

//returns the number of projectors that the ILDA specifies graphics for
size_t ilda_projector_count(ILDA* f);

//returns the number of frames for the given projector descriptor
size_t ilda_frame_count(ILDA* f, size_t pd);



/******************************************************************************/
/*  LZR ZeroMQ Facilities                                                     */
/******************************************************************************/

#define LZRD_GRAPHICS_ENDPOINT "tcp://127.0.0.1:5555"

//create a ZMQ transmitter (publisher)
void* frame_pub_new(void* zmq_ctx, const char* address);

//create a ZMQ reciever (subscriber)
void* frame_sub_new(void* zmq_ctx, const char* address);

//send a single frame
int send_frame(void* pub, const Frame& frame);

//recieve a single frame (blocking)
int recv_frame(void* sub, Frame& frame);


} // namespace lzr
