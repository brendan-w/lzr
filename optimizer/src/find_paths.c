

#include <stdio.h>
#include "optimize.h"


//forward declare
static void path_split(lzr_point_buffer* points, lzr_path_buffer* paths);
static void fill_angle(lzr_point_buffer* points, lzr_path_buffer* paths);
static void fill_cycle(lzr_point_buffer* points, lzr_path_buffer* paths);


void find_paths(lzr_point_buffer* points, lzr_path_buffer* paths)
{
    path_split(points, paths);
    fill_angle(points, paths);
    fill_cycle(points, paths);
}

static void path_split(lzr_point_buffer* points, lzr_path_buffer* paths)
{
    size_t n = 0;         //number of completed paths (index of the path currently being built)
    bool in_path = false; //whether the loop is inside an unterminated path

    for(size_t i = 0; i < points->length; i++)
    {
        lzr_point p = points->points[i];

        if(IS_BLANKED(p))
        {
            if(in_path)
            {
                //encountered first blanked point when IN a path
                //close the open path
                paths->paths[n].bi = i - 1; //store the index of the last point in the path
                n++;
                in_path = false;
            }
            // else, do nothing, discard blanked points
        }
        else //a lit point
        {
            if(in_path)
            {
                //test the angle this point makes with previous/next points
            }
            else
            {
                //encountered first lit point when not in a path
                //open a new path
                paths->paths[n].ai = i; //store the index of the first point in the path
                in_path = true;
            }
        }
    }

    //if a path is still open, close it
    if(in_path)
    {
        paths->paths[n].bi = points->length - 1;
        n++;
        in_path = false;
    }

    paths->length = n;
}

static void fill_angle(lzr_point_buffer* points, lzr_path_buffer* paths)
{
    for(size_t i = 0; i < paths->length; i++)
    {
        lzr_path* path = (paths->paths + i);

        switch(path->bi - path->ai)
        {
            case 0: //a single point
                //do nothing: single points can be approached from any direction
                path->a_angle = path->b_angle = 0;
                break;
            case 1: //two points
                path->a_angle = ANGLE(points->points[path->ai],
                                      points->points[path->bi]);
                path->b_angle = ANGLE_OPPOSITE(path->a_angle + PI);
                break;
            default: //more than two points
                path->a_angle = ANGLE(points->points[path->ai],
                                      points->points[path->ai + 1]);
                path->b_angle = ANGLE(points->points[path->bi],
                                      points->points[path->bi - 1]);
                break;
        }
    }
}

static void fill_cycle(lzr_point_buffer* points, lzr_path_buffer* paths)
{

}