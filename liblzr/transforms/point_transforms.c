

#include <math.h>
#include <lzr.h>


//run-of-the-mill linear interpolation
static inline double lerp(double v0, double v1, double t)
{
	return (1-(t))*(v0) + (t)*(v1);
}

//wrapper for lerping a uint8_t
static inline uint8_t lerp_uint8(uint8_t v0, uint8_t v1, double t)
{
	return (uint8_t) round(lerp((double) v0, (double) v1, t));
}


lzr_point lzr_point_lerp(lzr_point* a, lzr_point* b, double t)
{
    lzr_point p;
    p.x = lerp(a->x, b->x, t);
    p.y = lerp(a->y, b->y, t);
    p.r = lerp_uint8(a->r, b->r, t);
    p.g = lerp_uint8(a->g, b->g, t);
    p.b = lerp_uint8(a->b, b->b, t);
    p.i = lerp_uint8(a->i, b->i, t);
    return p;
}
