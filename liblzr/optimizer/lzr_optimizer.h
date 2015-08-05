

#ifndef OPTIMIZER_INTERNALS_H
#define OPTIMIZER_INTERNALS_H

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
    size_t      anchor_points;    //number of end points to place when performing blanking jumps
    opt_point_t last_known_point; //the last point from the previously optimized frame

    //point buffer
    opt_point_t* points; //array of points, the size of max_points
    size_t     n_points; //number of points currently in the buffer

    //path buffer
    opt_path_t*  paths;  //array of paths, the size of max_points
    size_t     n_paths;  //number of paths currently in the buffer
} opt_t;


#define zero_opt_point(p) (p)->angle = 0.0f;     \
                          (p)->base_point.x = 0; \
                          (p)->base_point.y = 0; \
                          (p)->base_point.r = 0; \
                          (p)->base_point.g = 0; \
                          (p)->base_point.b = 0; \
                          (p)->base_point.i = 0;


/*
    Splits a point buffer into individual path segments.
    Loads the result into the given path buffer.
*/
void find_paths(opt_t* opt);

/*
    Arranges the paths in the path buffer to minimize blank time,
    and route the laser in a sensical path.
*/
void rearrange_paths(opt_t* opt);

/*
    Fills the given output buffer according to the modified path buffer.
    Inserts/generates new blanking jumps. Returns the new size of the
    output buffer.
*/
int compile_paths(opt_t* opt, lzr_frame* frame);


#endif /* OPTIMIZER_INTERNALS_H */
