
#pragma once

#include <vector>
#include <cmath>
#include <lzr.h>

namespace lzr {



//tunable settings
#define PATH_SPLIT_ANGLE (PI / 8) //max angle deviation for points to be considered contiguous

//constants
#define PI 3.14159265358979323846

//functions
#define ANGLE(a, b)       ( std::atan2(b.y - a.y, b.x - a.y) * -1 ) //the angle from point A to point B on range (-PI, PI] in screen coords
#define ANGLE_NORM(a)     ( std::fmod(a, PI) ) //constrains angles to (-PI, PI]
#define ANGLE_OPPOSITE(a) ( ANGLE_NORM(a + PI) )



//specialized point class for handling the endpoints of a path
class Optimizer_Point {
public:
    Point point;  //copy of actual point data
    double angle; //the angle for entering the path at this point (radians)
};


//struct defining a "continguous" segment of the lasers points
class Optimizer_Path {
public:
    size_t a;   //index of the front point
    size_t b;   //index of the back point
    bool cycle; //whether or not this path is cyclic (EXPERIMENTAL)

    size_t size();
    void invert();
};


typedef struct {
    Optimizer_Path* path; //pointer to the path in the path buffer
    bool invert;      //whether or not the path should be entered at it's B point
} path_descriptor;



//optimizer context
class Optimizer_Context {
public:
    int run(Frame& frame, double split_angle,      //radians for path splitting tolerance
                          bool reorder,            //whether to reorder the paths
                          size_t lit_anchors,      //minimum number of lit anchors
                          size_t blanked_anchors); //minimum number of blanked anchors

private:
    //working data
    Optimizer_Point last_known_point;    //the last point from the previously optimized frame
    std::vector<Optimizer_Point> points; //point buffer
    std::vector<Optimizer_Path> paths;   //path buffer

    //Top level functions (these are order specific)

    /*
        Splits a point buffer into individual path segments.
        Loads the result into the given path buffer.
    */
    void find_paths(double split_angle);

    /*
        Arranges the paths in the path buffer to minimize blank time,
        and route the laser in a sensical path.
    */
    void reorder_paths();

    /*
        Fills the given output buffer according to the modified path buffer.
        Inserts/generates new blanking jumps. Returns the new size of the
        output buffer.
    */
    void compile_paths(Frame& frame, size_t lit, size_t blanked);



    //internal path-finding functions
    void fill_angle();
    void path_split(double split_angle);
    void fill_cycle(double split_angle);

    //path reorderring functions
    double cost(Optimizer_Point a, Optimizer_Point b);
    void swap_paths(Optimizer_Path* path_a, Optimizer_Path* path_b);
    path_descriptor find_next(size_t start, Optimizer_Point laser);
};



} // namespace lzr
