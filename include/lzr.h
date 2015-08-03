
#ifndef LZR_TYPES_H
#define LZR_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/*
    POINTS
*/

typedef struct {
    int16_t x;  //position X
    int16_t y;  //position Y
    uint16_t r; //Red
    uint16_t g; //Green
    uint16_t b; //Blue
    uint16_t i; //Blanking
} lzr_point;

#define LZR_POINT_MAX_POSITION 32768
#define LZR_POINT_MAX_COLOR    65535
#define LZR_POINT_SQ_DISTANCE(a, b) ( (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) ) //square of the distance between two points
#define LZR_POINT_BLANK(p)          p.r = p.g = p.b = p.i = 0
#define LZR_POINT_IS_BLANKED(p)     ( (p.i == 0) || (p.r + p.g + p.b == 0) ) // returns boolean for whether the given point is blanked
#define LZR_POINTS_SAME_POS(a, b)   ( (a.x == b.x) && (a.y == b.y) ) //are two points at the same position
#define LZR_POINTS_SAME_COLOR(a, b) ( (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.i == b.i) ) //are two points the same color
#define LZR_POINTS_EQUAL(a, b)      ( POINTS_SAME_POS(a, b) && POINTS_SAME_COLOR(a, b) )


/*
    FRAMES
*/

#define LZR_FRAME_MAX_POINTS 2000  // = 60,000 pps / 30 fps

typedef struct {
  lzr_point points[LZR_FRAME_MAX_POINTS];
  uint16_t n_points;
} lzr_frame;


/*
    TIME
*/

//sometimes this represents seconds, others, it represents normallized time
//Normalized time is ussually notated `nt` while time in seconds is simply `t`
typedef double lzr_time;



#ifdef __cplusplus
} // extern "c"
#endif

#endif /* LZR_TYPES_H */
