
#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <lzr.h>





//tunable settings
#define POINT_BUFFER_SIZE 1800    //
#define PATH_SPLIT_ANGLE (PI / 8) //max angle deviation for points to be considered contiguous


//computed settings
#define PATH_BUFFER_SIZE (POINT_BUFFER_SIZE / 2) //the shortest path is 2 points

//constants
#define PI 3.14159265358979323846

//functions
#define DISTANCE(a, b)        ( (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) ) //square of the distance between two points
#define ANGLE(a, b)           ( atan2(b.y - a.y, b.x - a.y) * -1 ) //the angle from point A to point B on range (-PI, PI] in screen coords
#define ANGLE_NORM(a)         ( fmod(a, PI) ) //constrains angles to (-PI, PI]
#define ANGLE_OPPOSITE(a)     ( ANGLE_NORM(a + PI) )


typedef double opt_angle_t;

//specialized point class for handling the endpoints of a path
typedef struct {
    lzr_point   base_point; //copy of actual point data
    opt_angle_t angle;      //the angle for entering the path at this point (radians)
} opt_point_t;

//struct defining a "continguous" segment of the lasers path
typedef struct {
    size_t a;   //index of the front point
    size_t b;   //index of the back point
    bool cycle; //whether or not this path is cyclic (EXPERIMENTAL)
} opt_path_t;



//optimizer context
typedef struct {
    size_t      max_points;       //the max number of points the buffers can hold
    size_t      anchor_points;    //number of end points to place when performing blankin jumps
    opt_point_t last_known_point; //the last point from the previously optimized frame

    //point buffer
    opt_point_t* points; //array of points, the size of max_points
    size_t     n_points; //number of points currently in the buffer

    //path buffer
    opt_path_t*  paths;  //array of paths, the size of max_points
    size_t     n_paths;  //number of paths currently in the buffer
} lzr_optimizer;


#define zero_opt_point(p) (p)->angle = 0.0f;     \
                          (p)->base_point.x = 0; \
                          (p)->base_point.y = 0; \
                          (p)->base_point.r = 0; \
                          (p)->base_point.g = 0; \
                          (p)->base_point.b = 0; \
                          (p)->base_point.i = 0;



/*
    Allocates and returns a point to a new optimizer context.
*/
lzr_optimizer* lzr_create_optimizer(size_t max_points);

/*
    Deallocator for the optimizer context
*/
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


#endif /* OPTIMIZE_H */
