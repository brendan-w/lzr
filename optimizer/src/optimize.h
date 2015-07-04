
#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <lzr_types.h>

//tunable settings
#define POINT_BUFFER_SIZE 10        //
#define PATH_SPLIT_ANGLE (180 - 30) //max number of degrees before the point is considered a corner

//computed settings
#define PATH_BUFFER_SIZE (POINT_BUFFER_SIZE / 2) //the shortest path is 2 points


//functions
#define DISTANCE(a, b) ( (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) ) //square of the distance between two points
#define IS_BLANKED(p)  ( (p.i == 0) || (p.r + p.g + p.b == 0) ) // returns boolean for whether the given point is blanked


//struct defining a "continguous" segment of the lasers path
typedef struct {
    //order of start vs. end points is irrelevant here
    uint16_t ai;      //index of a starting point (or, the end point)
    uint16_t bi;      //index of a ending point (or, the start point)
    uint16_t a_angle; //the entrance angle of laser going into the path
    uint16_t b_angle; //the exit angle of the laser as it leaves the path
    bool cycle;       //whether or not this path is cyclic (points A and B are equal)
    bool used;        //whether or not the algorithm has handled this segment before
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
