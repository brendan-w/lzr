
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


#endif /* LZR_TYPES_H */
