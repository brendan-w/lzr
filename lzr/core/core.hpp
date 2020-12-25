/******************************************************************************/
/*                                                                            */
/*  liblzr - a suite of common tools for handling laser graphics              */
/*                                                                            */
/******************************************************************************/

#pragma once

// Windows dll export symbols
#ifdef _WIN32
#define LIBLZR_EXPORT __declspec(dllexport)
#else
#define LIBLZR_EXPORT
#endif


#include <stdint.h>
#include <stdbool.h>
#include <vector>
#include <set>
#include <string>

namespace lzr {

/******************************************************************************/
/*  LZR Return Codes                                                          */
/******************************************************************************/

#define LZR_SUCCESS            0
#define LZR_WARNING           -1
#define LZR_FAILURE           -2
#define LZR_ERROR_INVALID_ARG -3



/******************************************************************************/
/*  LZR Points                                                                */
/******************************************************************************/


/*
 * Coordinate system:
 *
 *  (-1, 1)        (1, 1)
 *      _____________
 *     |      |      |
 *     |      |      |
 *     |      |      |
 *     |------+------|
 *     |      |      |
 *     |      |      |
 *     |______|______|
 *
 * (-1, -1)        (1, -1)
 *
 */

class LIBLZR_EXPORT Point
{
public:
    //limits
    static constexpr float POSITION_MIN = -1.0;
    static constexpr float POSITION_MAX = 1.0;
    static constexpr uint8_t COLOR_MIN  = 0;
    static constexpr uint8_t COLOR_MAX  = 255;

    float x = 0.0; // Position X   [-1.0, 1.0]
    float y = 0.0; // Position Y   [-1.0, 1.0]
    uint8_t r = 0; // Red          [0, 255]
    uint8_t g = 0; // Green        [0, 255]
    uint8_t b = 0; // Blue         [0, 255]
    uint8_t i = 0; // Blanking     [0, 255]
    bool beam = false; // Bright single-point beam. This is a meta-parameter that is consumed by the
                       // optimizer to indicate that it should stack multiple pionts in the final laser
                       // output to create a single bright beam in the image. The intensity of the beam
                       // is governed by "i" as normal, and the exact point counts used are a parameter
                       // of the optimizer.

    Point() = default;
    Point(float x, float y)
      : x(x)
      , y(y)
    {}
    Point(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t i)
      : x(x)
      , y(y)
      , r(r)
      , g(g)
      , b(b)
      , i(i)
    {}

    /**
     * Trivial setters and getters
     */
    void blank() { i = 0; }
    void unblank() { i = 222; }
    bool is_blanked() const { return (i == 0) || (r + g + b == 0); }
    bool is_lit() const { return (i > 0) && (r + g + b > 0); }

    void set_position(const Point& other)
    {
        x = other.x;
        y = other.y;
    }

    void set_color(const Point& other)
    {
        r = other.r;
        g = other.g;
        b = other.b;
        i = other.i;
        beam = other.beam;
    }

    bool same_position_as(const Point& other) const;
    bool same_color_as(const Point& other) const;

    bool operator==(const Point& other) const
    {
        return (same_position_as(other) &&
                same_color_as(other));
    }

    bool operator!=(const Point& other) const
    {
        return !operator==(other);
    }

    /**
     * Computation and point generation
     */
    Point lerp_to(const Point& other, float t) const;
    float distance_to(const Point& other) const;
    float sq_distance_to(const Point& other) const;
};



/******************************************************************************/
/*  LZR Frames                                                                */
/******************************************************************************/

//OK as long as the Frame class doesn't save any state, which, it shouldn't...
//this is only a convenient way to attach functions to the type
class LIBLZR_EXPORT Frame : public std::vector<Point>
{
public:
    Frame() : std::vector<Point>() {};
    Frame(size_t n) : std::vector<Point>(n) {};

    Frame& add(const Point& p);
    Frame& add(const Frame& other);
    Frame& add_with_blank_jump(const Point& p);
    Frame& add_with_blank_jump(const Frame& other);

    void bounding_box(Point& min, Point& max) const;
    Point bounding_box_center() const;
    Point average_center() const;

private:
    Frame& add_blank_jump_to(const Point& p);
};


/******************************************************************************/
/*  LZR Animations & Partial Frames                                           */
/******************************************************************************/

typedef std::vector<Frame> FrameList;

//splits a single frame into lit paths.
FrameList split_frame(const Frame& frame);
Frame combine_frames(const FrameList& frames);


/******************************************************************************/
/*  LZR Frame Transforms                                                      */
/******************************************************************************/

LIBLZR_EXPORT int translate(Frame& frame, float x, float y);
LIBLZR_EXPORT int rotate(Frame& frame, Point center, float theta);
LIBLZR_EXPORT int scale(Frame& frame, Point center, float x, float y);
LIBLZR_EXPORT int mirror(Frame& frame, Point center, bool x, bool y);
LIBLZR_EXPORT int dup_mirror(Frame& frame, Point center, bool x, bool y, bool blank=true);
LIBLZR_EXPORT int dup_linear(Frame& frame, Point offset, size_t n_dups, bool blank=true);
LIBLZR_EXPORT int dup_radial(Frame& frame, Point center, size_t n_dups, float angle, bool blank=true);


//clips a frame using the given mask. Points in the mask should define a closed
//polygon. All points outside the mask are discarded. Line segments that cross
//the mask boundry will have additional points inserted at that boundry.
LIBLZR_EXPORT int mask(Frame& frame, Frame mask, bool inverse=false);



/******************************************************************************/
/*  LZR Interpolation                                                         */
/******************************************************************************/

//interpolation functions
typedef float (*interpolation_func)(float t);
LIBLZR_EXPORT float linear(float t); /*----*----*----*----*----*----*----*----*/
LIBLZR_EXPORT float quad(float t);   /*---*---*-----*-----*-----*-----*---*---*/
LIBLZR_EXPORT float quart(float t);  /*-*---*-----*-------*-------*-----*---*-*/

//generates extra points on a line
//only adds the interior points, not the endpoints
int interp_line(Frame& working, float max_distance, interpolation_func func, Point start, Point end);

//interpolates only lit lines. Uses the function above
int interpolate(Frame& frame, float max_distance, interpolation_func func);



/******************************************************************************/
/*  LZR Reducers (inverse functions for the optimization above)               */
/******************************************************************************/

//deletes unwanted points from the given frame

//removes duplicate lit points (anchor points and beams)
LIBLZR_EXPORT int reduce_duplicates(Frame& frame);

//removes interpolation from straight, lit lines
LIBLZR_EXPORT int reduce_interpolation(Frame& frame);

//removes all intersticial blanked points, regardless of path
LIBLZR_EXPORT int reduce_blanks(Frame& frame);

} // namespace lzr
