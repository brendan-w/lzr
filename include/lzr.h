
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

#define POSITION_MAX 65535
#define COLOR_MAX    65535

#define ANTINORM_POSITION(nv) (nv / POSITION_MAX) //de-normalizes a value ( [0, 1] ---> [0, POSITION_MAX] )
#define ANTINORM_COLOR(nv)    (nv / COLOR_MAX)

#define POINT_BLANK(p)          p.r = p.g = p.b = p.i = 0
#define POINT_BLANKED(p)        ( (p.i == 0) || (p.r + p.g + p.b == 0) ) // returns boolean for whether the given point is blanked
#define POINTS_SAME_POS(a, b)   ( (a.x == b.x) && (a.y == b.y) ) //are two points at the same position
#define POINTS_SAME_COLOR(a, b) ( (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.i == b.i) ) //are two points the same color
#define POINTS_EQUAL(a, b)      ( POINTS_SAME_POS(a, b) && POINTS_SAME_COLOR(a, b) )


/*
    FRAMES
*/

#define FRAME_MAX_POINTS 1800

typedef struct {
  lzr_point points[FRAME_MAX_POINTS];
  uint16_t n_points;
} lzr_frame;


#ifdef __cplusplus
} // extern "c"
#endif

#endif /* LZR_TYPES_H */
