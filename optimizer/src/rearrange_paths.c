

#include "optimize.h"
#include "rearrange_paths.h"



//forward declare
static size_t find_next(lzr_path_buffer* paths, size_t start);
static void swap_paths(lzr_path_buffer* paths, size_t a, size_t b);
static size_t cost(lzr_path_point a, lzr_path_point b);


void rearrange_paths(lzr_point_buffer* points, lzr_path_buffer* paths)
{
    lzr_path_point current; //current position/angle of the laser
    current.point.x = 0;
    current.point.y = 0;
    current.angle = 0.0;

    for(size_t i = 0; i < paths->length; i++)
    {
        size_t min;

    }
}

//scan for the best path to enter next
static size_t find_next(lzr_path_buffer* paths, size_t start)
{
        for(size_t i = start; i < paths->length; i++)
        {

            // size_t c = cost(a, b);
        }

}

//the cost function for a blank jump between points A and B 
static size_t cost(lzr_path_point a, lzr_path_point b)
{
    return DISTANCE(a.point, b.point);
}

static void swap_paths(lzr_path_buffer* paths, size_t a, size_t b)
{
    lzr_path temp = paths->paths[a];
    paths->paths[a] = paths->paths[b];
    paths->paths[b] = temp;
}
