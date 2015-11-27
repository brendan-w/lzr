
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
};


//optimizer context
class Optimizer_Context {
public:
    void load_points(Frame& frame);
    void find_paths(double split_angle);

    Optimizer_Point last_known_point;    //the last point from the previously optimized frame
    std::vector<Optimizer_Point> points; //point buffer
    std::vector<Optimizer_Path> paths;   //path buffer

private:
    //path-finding functions
    void fill_angle();
    void path_split(double split_angle);
    void fill_cycle(double split_angle);
};






/*
        Arranges the paths in the path buffer to minimize blank time,
        and route the laser in a sensical path.
*/
// void rearrange_paths(opt_t* opt);

/*
        Fills the given output buffer according to the modified path buffer.
        Inserts/generates new blanking jumps. Returns the new size of the
        output buffer.
*/
// int compile_paths(opt_t* opt, lzr_frame* frame);


} // namespace lzr
