
#ifndef LZR_TYPES_H
#define LZR_TYPES_H

#include <stdint.h>



typedef struct {
    uint16_t x; //position X
    uint16_t y; //position Y
    uint16_t r; //Red
    uint16_t g; //Green
    uint16_t b; //Blue
    uint16_t i; //Blanking
} lzr_point;

#define POSITION_MAX 65535
#define COLOR_MAX    65535

#define ANTINORM_POSITION(nv) (nv / POSITION_MAX) //un-normalizes a value ( [0, 1] ---> [0, POSITION_MAX] )
#define ANTINORM_COLOR(nv)    (nv / COLOR_MAX)

#define POINT_INIT(_x, _y, _r, _g, _b, _i) { .x=_x, .y=_y, .r=_r, .g=_g, .b=_b, .i=_i }
#define POINT_INIT_ZERO() { .x=0, .y=0, .r=0, .g=0, .b=0, .i=0 }
#define POINT_BLANK(p) p.r = p.g = p.b = p.i = 0
#define POINT_BLANKED(p)        ( (p.i == 0) || (p.r + p.g + p.b == 0) ) // returns boolean for whether the given point is blanked
#define POINTS_SAME_POS(a, b)   ( (a.x == b.x) && (a.y == b.y) ) //are two points at the same position
#define POINTS_SAME_COLOR(a, b) ( (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.i == b.i) ) //are two points the same color
#define POINTS_EQUAL(a, b)      ( POINTS_SAME_POS(a, b) && POINTS_SAME_COLOR(a, b) )



typedef struct {
  size_t n_points;
  lzr_point* points;
} lzr_frame;


#endif /* LZR_TYPES_H */
