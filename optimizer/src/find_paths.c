

#include "optimize.h"
#include "find_paths.h"


//store the index of the last point in the path
#define CLOSE_PATH(p, i) paths->paths[n].b.point  = (p);\
                         paths->paths[n].b.i = (i);\
                         n++;\
                         in_path = false;\


//forward declare
static void path_split(lzr_point_buffer* points, lzr_path_buffer* paths);
static void fill_angle(lzr_point_buffer* points, lzr_path_buffer* paths);
// static void fill_cycle(lzr_point_buffer* points, lzr_path_buffer* paths);


void find_paths(lzr_point_buffer* points, lzr_path_buffer* paths)
{
    path_split(points, paths);
    fill_angle(points, paths);
    // fill_cycle(points, paths);
}

static void path_split(lzr_point_buffer* points, lzr_path_buffer* paths)
{
    size_t n = 0;         //number of completed paths (index of the path currently being built)
    bool in_path = false; //whether the loop is inside an unterminated path
    lzr_point p;          //the current point being checked

    for(size_t i = 0; i < points->length; i++)
    {
        p = points->points[i];

        if(POINT_BLANKED(p))
        {
            if(in_path)
            {
                //encountered first blanked point when IN a path
                //close the open path
                paths->paths[n].b.point = p;
                paths->paths[n].b.i = i - 1;
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
                //TODO: clean this up

                if((i - paths->paths[n].a.i > 0) &&                                 //is there a previous point to check against
                   (i+1 < points->length) && !POINT_BLANKED(points->points[i + 1])) //is the next point valid to check against
                {
                    lzr_point prev_p = points->points[i - 1];
                    lzr_point next_p = points->points[i + 1];

                    //if it creates too much of an angle
                    if(ANGLE_FORMED(prev_p, p, next_p) > PATH_SPLIT_ANGLE)
                    {
                        //close the current path
                        paths->paths[n].b.point = p;
                        paths->paths[n].b.i = i;
                        n++;
                        //open a new one
                        paths->paths[n].a.point = p;
                        paths->paths[n].a.i = i;
                    }
                }
            }
            else
            {
                //encountered first lit point when not in a path
                //open a new path
                paths->paths[n].a.point = p;
                paths->paths[n].a.i = i; //store the index of the first point in the path
                in_path = true;
            }
        }
    }

    //if a path is still open, close it
    if(in_path)
    {
        paths->paths[n].b.point = p;
        paths->paths[n].b.i = points->length - 1;
        n++;
        in_path = false;
    }

    paths->length = n;
}

//computes the entrance angles on either side of each path
static void fill_angle(lzr_point_buffer* points, lzr_path_buffer* paths)
{
    for(size_t i = 0; i < paths->length; i++)
    {
        lzr_path* path = (paths->paths + i);

        switch(path->b.i - path->a.i)
        {
            case 0: //a single point
                //do nothing: single points can be approached from any direction
                path->a.angle = path->b.angle = 0;
                break;
            case 1: //two points
                path->a.angle = ANGLE(points->points[path->a.i],
                                      points->points[path->b.i]);
                path->b.angle = ANGLE_OPPOSITE(path->a.angle + PI);
                break;
            default: //more than two points
                path->a.angle = ANGLE(points->points[path->a.i],
                                      points->points[path->a.i + 1]);
                path->b.angle = ANGLE(points->points[path->b.i],
                                      points->points[path->b.i - 1]);
                break;
        }
    }
}

/*
static void fill_cycle(lzr_point_buffer* points, lzr_path_buffer* paths)
{
    for(size_t i = 0; i < paths->length; i++)
    {
        lzr_path* path = (paths->paths + i);
        lzr_point a = points->points[path->a.i];
        lzr_point b = points->points[path->b.i];

        //if they're in the same position, and there are at least 3 points
        if(POINTS_SAME_POS(a, b) && (path->b.i - path->a.i > 1))
        {
            //fetch the points on either side of the joint
            lzr_point next = points->points[path->a.i + 1];
            lzr_point prev = points->points[path->b.i - 1];

            //if it DOESN'T creates too much of an angle, then it's a cycle
            path->cycle = (ANGLE_FORMED(prev, a, next) <= PATH_SPLIT_ANGLE);
        }
    }
}
*/
