

#include "rearrange_paths.h"


typedef struct {
    size_t i;    //index of the path in the path buffer
    bool invert; //whether or not the path should be entered at it's B point
} path_descriptor;


//forward declare
static path_descriptor find_next(lzr_path_buffer* paths, size_t start, opt_point current);
static size_t cost(opt_point a, opt_point b);
static void swap_paths(lzr_path_buffer* paths, size_t a, size_t b);
static void invert_path(lzr_path_buffer* paths, size_t i);


void rearrange_paths(lzr_point_buffer* points, lzr_path_buffer* paths)
{
    opt_point current; //current position/angle of the laser
    current.point.x = 0;
    current.point.y = 0;
    current.angle = 0.0;

    for(size_t i = 0; i < paths->length; i++)
    {
        path_descriptor next = find_next(paths, i, current);
        swap_paths(paths, i, next.i);
        if(next.invert)
            invert_path(paths, next.i);

        //update the laser's current location
        current = paths->paths[i].b;
    }
}

//scan for the best path to enter next
static path_descriptor find_next(lzr_path_buffer* paths, size_t start, opt_point current)
{

    //optimize for least cost
    size_t min_cost = 0;

    //running vars
    path_descriptor choice; //the best path
    opt_point possible;
    size_t c;

    //initiail check
    possible = paths->paths[start].a;
    c = cost(current, possible);
    min_cost = c;
    choice.i = start;
    choice.invert = false;

    
    for(size_t i = start; i < paths->length; i++)
    {

        //test the front point (A)
        possible = paths->paths[i].a;
        c = cost(current, possible);
        if(c < min_cost)
        {
            min_cost = c;
            choice.i = i;
            choice.invert = false;
        }

        //test the back point (B)
        possible = paths->paths[i].b;
        c = cost(current, possible);
        if(c < min_cost)
        {
            min_cost = c;
            choice.i = i;
            choice.invert = true;
        }
    }

    return choice;
}

//the cost function for a blank jump between points A and B 
static size_t cost(opt_point a, opt_point b)
{
    return DISTANCE(a.point, b.point);
}

static void swap_paths(lzr_path_buffer* paths, size_t a, size_t b)
{
    if(a != b)
    {
        lzr_path temp = paths->paths[a];
        paths->paths[a] = paths->paths[b];
        paths->paths[b] = temp;
    }
}

static void invert_path(lzr_path_buffer* paths, size_t i)
{
    opt_point temp = paths->paths[i].a;
    paths->paths[i].a = paths->paths[i].b;
    paths->paths[i].b = temp;
}
