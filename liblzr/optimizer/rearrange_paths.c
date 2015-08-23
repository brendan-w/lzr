

#include "lzr_optimizer.h"

/*
    Returns a pointer to an opt_point_t.
    Supply a pointer to a path, and a choice of the A or B point.
*/

#define GET_POINT_PTR(i) (opt->points + (i))
#define GET_PATH_PTR(i) (opt->paths + (i))

typedef struct {
    opt_path_t* path; //pointer to the path in the path buffer
    bool invert;      //whether or not the path should be entered at it's B point
} path_descriptor;


//the cost function for a blank jump between points A and B
static double cost(opt_point_t* a, opt_point_t* b)
{
    int64_t sq_dist = LZR_POINT_SQ_DISTANCE(a->base_point, b->base_point);
    double dist     = (double) round(sqrt((double) sq_dist));
    return dist;
}

static void swap_paths(opt_path_t* path_a, opt_path_t* path_b)
{
    if(path_a != path_b)
    {
        opt_path_t temp = *path_a;
        *path_a = *path_b;
        *path_b = temp;
    }
}

static void invert_path(opt_path_t* path)
{
    size_t temp = path->a;
    path->a = path->b;
    path->b = temp;
}

//scan for the best path to enter next
static path_descriptor find_next(opt_t* opt, size_t start, opt_point_t* laser)
{
    //running vars
    path_descriptor pd;    //the best path
    opt_point_t* possible; //temp var for testing a path
    double c;              //temp var for computing cost
    double min_cost = 0.0; //optimize for least cost


    //initiail check
    possible = GET_POINT_PTR(opt->paths[start].a);
    min_cost = c = cost(laser, possible);
    pd.path = GET_PATH_PTR(start);
    pd.invert = false;
    
    for(size_t i = start; i < opt->n_paths; i++)
    {
        opt_path_t* current = GET_PATH_PTR(i);

        //test the front point (A)
        possible = GET_POINT_PTR(opt->paths[i].a);
        c = cost(laser, possible);
        if(c < min_cost)
        {
            min_cost = c;
            pd.path = current;
            pd.invert = false;
        }

        //test the back point (B)
        possible = GET_POINT_PTR(opt->paths[i].b);
        c = cost(laser, possible);
        if(c < min_cost)
        {
            min_cost = c;
            pd.path = current;
            pd.invert = true;
        }
    }

    return pd;
}

void rearrange_paths(opt_t* opt)
{
    opt_point_t laser = opt->last_known_point;

    for(size_t i = 0; i < opt->n_paths; i++)
    {
        opt_path_t* current = GET_PATH_PTR(i);

        path_descriptor next = find_next(opt, i, &laser);
        swap_paths(current, next.path);

        if(next.invert)
            invert_path(current);

        //update the laser's current location
        laser = opt->points[current->b];
    }
}

