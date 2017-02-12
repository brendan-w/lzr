
#pragma once

#include <vector>
#include <cmath>
#include <liblzr.hpp>

namespace lzr {



//constants
#define PI 3.14159265358979323846
#define ANGLE_ANY 0.0 //angle used to denote no angle preference

//functions
#define DEG_TO_RAD(deg)   ((deg) * PI / 180.0)
#define ANGLE_NORM(a)     ( std::fmod(a, PI) ) //constrains angles to (-PI, PI]
#define ANGLE_OPPOSITE(a) ( ANGLE_NORM(a + PI) )

//the angle from point A to point B on range (-PI, PI] in screen coords
#define POINT_ANGLE(a, b) ( (a.x == b.x) && (a.y == b.y) ? ANGLE_ANY : \
                                                           std::atan2(b.y - a.y, b.x - a.y) * -1 )

/*
    The angle formed between three points. Because of the way point
    angles are stored, only points B and C are neccessary.
    (see fill_angle() for details)

              C .
               /
              /
    A      B /
    ._______.

    This macros reports the angle at which the figure DEVIATES from a
    straight line:

                .
               /
              /
             /  ) theta
    ._______.  .  .  .  .

*/
#define ANGLE_DEFLECTION(b, c) (std::abs(c - b)) //for individual angle values
#define POINT_DEFLECTION(b, c) (ANGLE_DEFLECTION(b.angle, c.angle)) //for points themselves



/*
 * specialized point class for with extra fields for our purposes
 */
class Optimizer_Point : public Point
{
public:
    double angle; //the angle for entering the path at this point (radians)

    const Optimizer_Point& operator=(const Point& rhs); //for Optimizer_Point = Point
    Point to_point();
};



/*
 * struct defining a continguous segment of laser light
 */
class Optimizer_Path
{
public:
    Optimizer_Path(size_t _a, size_t _b, const std::vector<Optimizer_Point> & points);

    //always pass the points array when actual point data is needed
    //otherwise, NEVER retain a reference, because some data will need to persist
    //(ie. the last_known_point)
    size_t size() const;
    void invert();
    double entrance_angle() const;
    double exit_angle() const;
    size_t front_anchors(const std::vector<Optimizer_Point> & points) const;
    size_t back_anchors(const std::vector<Optimizer_Point> & points) const;
    Optimizer_Point front(const std::vector<Optimizer_Point> & points) const;
    Optimizer_Point back(const std::vector<Optimizer_Point> & points) const;
    Optimizer_Point at(size_t n, const std::vector<Optimizer_Point> & points) const;

    bool cycle; //whether or not this path is cyclic (EXPERIMENTAL)

private:
    size_t a; //index of the front point
    size_t b; //index of the back point
    double entrance; //angle
    double exit; //angle
};



/*
 * private optimizer context
 */
class OptimizerInternals
{
public:
    OptimizerInternals(Point current_laser_position);

    //main optimization function. Accepts pointer for
    //the wrapper/settings holder object.
    int run(Optimizer* settings, Frame& frame);

private:
    //main frame buffers
    std::vector<Optimizer_Point> points; //point buffer
    std::vector<Optimizer_Path> paths;   //path buffer

    //state, preserved between frames
    Optimizer_Point last_known_point; //the last point from the previously optimized frame
    size_t num_last_known_anchors;    //number of ending anchor points on the previous frame



    //Top level functions (these are order specific)

    /*
        Splits a point buffer into individual path segments.
        Loads the result into the given path buffer.
    */
    void find_paths(Optimizer* settings);

    void fill_angle();
    void path_split(double split_angle);
    void fill_cycle(double split_angle);


    /*
        Arranges the paths in the path buffer to minimize blank time,
        and route the laser in a sensical path.
    */
    void reorder_paths(Optimizer* settings);

    double angular_deflection(const Optimizer_Point& laser, const Optimizer_Path& path);
    double cost(const Optimizer_Point laser, const Optimizer_Path path);
    void find_next_and_swap(const size_t current_path, const Optimizer_Point laser);


    /*
        Fills the given output buffer according to the modified path buffer.
        Inserts/generates new blanking jumps. Returns the new size of the
        output buffer.
    */
    void compile_paths(Optimizer* settings, Frame& frame);

    void add_path_to_frame(Optimizer* settings,
                           Frame& frame,
                           Optimizer_Path path);
    void blank_between(Optimizer* settings,
                       Frame& frame,
                       Point a,
                       Point b);
};


} // namespace lzr
