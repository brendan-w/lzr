
#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <lzr.h>


//tunable settings
#define POINT_BUFFER_SIZE 1000    //
#define PATH_SPLIT_ANGLE (PI / 8) //max angle deviation for points to be considered contiguous


//computed settings
#define PATH_BUFFER_SIZE (POINT_BUFFER_SIZE / 2) //the shortest path is 2 points

//constants
# define PI 3.14159265358979323846

//functions
#define DISTANCE(a, b)        ( (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) ) //square of the distance between two points
#define ANGLE(a, b)           ( atan2(b.y - a.y, b.x - a.y) * -1 ) //the angle from point A to point B on range (-PI, PI] in screen coords
#define ANGLE_NORM(a)         ( fmod(a, PI) ) //constrains angles to (-PI, PI]
#define ANGLE_OPPOSITE(a)     ( ANGLE_NORM(a + PI) )
#define ANGLE_FORMED(a, b, c) ( ANGLE(a, b) - ANGLE(b, c) ) //angle formed by three points [0, PI] (0 = straight line, PI = folded backward)


typedef double lzr_angle;

//specialized point class for handling the endpoints of a path
typedef struct {
    lzr_point point;       //copy of actual point data
    size_t i;          //index of the point in the point buffer
    lzr_angle angle; //the angle for entering the path at this point (radians)
} lzr_path_point;

//struct defining a "continguous" segment of the lasers path
typedef struct {
    lzr_path_point a;
    lzr_path_point b;    
    // bool cycle;     //whether or not this path is cyclic (points A and B are equal) (EXPERIMENTAL)
} lzr_path;

//point buffer
typedef struct {
    lzr_point points[POINT_BUFFER_SIZE];
    size_t length;
} lzr_point_buffer;

//path buffer
typedef struct {
    lzr_path paths[PATH_BUFFER_SIZE];
    size_t length;
} lzr_path_buffer;


#endif /* OPTIMIZE_H */
