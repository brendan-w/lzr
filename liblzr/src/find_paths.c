

#include <stdio.h>
#include "lzr_optimizer.h"

/*
    The angle formed between three points. Because of the way point
    angles are stored, only points B and C are neccessary.
    (see fill_angle() for details)

              C .
               /
              /
    A      B /
    ._______.
*/
#define ANGLE_FORMED(b, c) (abs(b.angle - c.angle))


//computes the angles from one point to the next
static void fill_angle(opt_t* opt)
{
    /*

        When computing the angles from one point to the next,
        the angle stored on each point is the INCOMING angle for that point.
        For the outgoing angle, use the value from the next point.
        The first point's angle is computed based on the last_known_point.

                                   angle
                |---->|           |---->|

                *    [P     P     P     P     P     P     ...]

                |                       |
        last_known_point           stored here

    */

    opt->points[0].angle = ANGLE(opt->last_known_point.base_point,
                                 opt->points[0].base_point);

    for(size_t i = 1; i < opt->n_points; i++)
    {
        opt->points[i].angle = ANGLE(opt->points[i - 1].base_point,
                                     opt->points[i].base_point);
    }
}


static void path_split(opt_t* opt)
{
    size_t n = 0;         //number of completed paths (index of the path currently being built)
    bool in_path = false; //whether the loop is inside an unterminated path
    opt_point_t p;        //the current point being checked

    for(size_t i = 0; i < opt->n_points; i++)
    {
        p = opt->points[i];

        if(LZR_POINT_IS_BLANKED(p.base_point))
        {
            if(in_path)
            {
                //encountered first blanked point when IN a path
                //close the open path
                opt->paths[n].b = i - 1;
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

                if((i+1 < opt->n_points) && !LZR_POINT_IS_BLANKED(opt->points[i + 1].base_point)) //is the next point valid to check against
                {
                    opt_point_t next = opt->points[i + 1];

                    //if it creates too much of an angle
                    if(ANGLE_FORMED(p, next) > PATH_SPLIT_ANGLE)
                    {
                        //close the current path
                        opt->paths[n].b = i;
                        n++;
                        //open a new one
                        opt->paths[n].a = i;
                    }
                }
            }
            else
            {
                //encountered first lit point when not in a path
                //open a new path
                opt->paths[n].a = i; //store the index of the first point in the path
                in_path = true;
            }
        }
    }

    //if a path is still open, close it
    if(in_path)
    {
        opt->paths[n].b = opt->n_points - 1;
        n++;
        in_path = false;
    }

    opt->n_paths = n;
}


static void fill_cycle(opt_t* opt)
{
    for(size_t i = 0; i < opt->n_paths; i++)
    {
        opt_path_t* path = (opt->paths + i);
        opt_point_t a = opt->points[path->a];
        opt_point_t b = opt->points[path->b];

        //if they're in the same position, and there are at least 3 points
        if(LZR_POINTS_SAME_POS(a.base_point, b.base_point) && (path->b - path->a > 1))
        {
            //fetch the point one forward of the joint
            opt_point_t next = opt->points[path->a + 1];

            //if it DOESN'T creates too much of an angle, then it's a cycle
            path->cycle = (ANGLE_FORMED(b, next) <= PATH_SPLIT_ANGLE);
        }
    }
}


//public function
void find_paths(opt_t* opt)
{
    fill_angle(opt);
    path_split(opt);
    fill_cycle(opt);
}
