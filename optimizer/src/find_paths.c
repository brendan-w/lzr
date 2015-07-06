

#include "find_paths.h"

#define OPT_POINT_BLANKED(p) POINT_BLANKED(p.base_point)
#define OPT_ANGLE_FORMED(a, b, c) ANGLE_FORMED(a.base_point, b.base_point, c.base_point)


static void path_split(lzr_optimizer* opt)
{
    size_t n = 0;         //number of completed paths (index of the path currently being built)
    bool in_path = false; //whether the loop is inside an unterminated path
    opt_point_t p;        //the current point being checked

    for(size_t i = 0; i < opt->n_points; i++)
    {
        p = opt->points[i];

        if(OPT_POINT_BLANKED(p))
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

                if((i - opt->paths[n].a > 0) &&                                 //is there a previous point to check against
                   (i+1 < opt->n_points) && !OPT_POINT_BLANKED(opt->points[i + 1])) //is the next point valid to check against
                {
                    opt_point_t prev = opt->points[i - 1];
                    opt_point_t next = opt->points[i + 1];

                    //if it creates too much of an angle
                    if(OPT_ANGLE_FORMED(prev, p, next) > PATH_SPLIT_ANGLE)
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

//computes the entrance angles on either side of each path
/*
static void fill_angle(lzr_optimizer* opt)
{
    for(size_t i = 0; i < opt->n_paths; i++)
    {
        opt_path* path = (paths->paths + i);

        switch(path->b - path->a)
        {
            case 0: //a single point
                //do nothing: single points can be approached from any direction
                path->a.angle = path->b.angle = 0;
                break;
            case 1: //two points
                path->a.angle = ANGLE(opt->points[path->a],
                                      opt->points[path->b]);
                path->b.angle = ANGLE_OPPOSITE(path->a.angle + PI);
                break;
            default: //more than two points
                path->a.angle = ANGLE(opt->points[path->a],
                                      opt->points[path->a + 1]);
                path->b.angle = ANGLE(opt->points[path->b],
                                      opt->points[path->b - 1]);
                break;
        }
    }
}
*/

/*
static void fill_cycle(lzr_optimizer* opt)
{
    for(size_t i = 0; i < opt->n_paths; i++)
    {
        opt_path* path = (paths->paths + i);
        opt_point_t a = opt->points[path->a];
        opt_point_t b = opt->points[path->b];

        //if they're in the same position, and there are at least 3 points
        if(POINTS_SAME_POS(a, b) && (path->b - path->a > 1))
        {
            //fetch the points on either side of the joint
            opt_point_t next = opt->points[path->a + 1];
            opt_point_t prev = opt->points[path->b - 1];

            //if it DOESN'T creates too much of an angle, then it's a cycle
            path->cycle = (ANGLE_FORMED(prev, a, next) <= PATH_SPLIT_ANGLE);
        }
    }
}
*/


//public function
void find_paths(lzr_optimizer* opt)
{
    path_split(opt);
    // fill_angle(opt);
    // fill_cycle(opt);
}
