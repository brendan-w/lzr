

#include "rearrange_paths.h"


typedef struct {
    opt_path_t* path; //pointer to the path in the path buffer
    bool invert;      //whether or not the path should be entered at it's B point
} path_descriptor;


//the cost function for a blank jump between points A and B
static size_t cost(opt_point_t* a, opt_point_t* b)
{
    return DISTANCE(a->base_point, b->base_point);
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
static path_descriptor find_next(lzr_optimizer* opt, size_t start)
{
    //optimize for least cost
    size_t min_cost = 0;

    //running vars
    path_descriptor pd;    //the best path
    opt_point_t* possible; //temp var for testing a path
    size_t c;              //temp var for computing cost


    //initiail check
    possible = (opt->paths[start].a);
    c = cost(opt->last_known_point, possible);
    min_cost = c;
    pd.path = possible;
    pd.invert = false;
    
    for(size_t i = start; i < paths->length; i++)
    {

        //test the front point (A)
        possible = paths->paths[i].a;
        c = cost(opt->last_known_point, possible);
        if(c < min_cost)
        {
            min_cost = c;
            pd.path = possible;
            pd.invert = false;
        }

        //test the back point (B)
        possible = paths->paths[i].b;
        c = cost(opt->last_known_point, possible);
        if(c < min_cost)
        {
            min_cost = c;
            pd.path = possible;
            pd.invert = true;
        }
    }

    return pd;
}

void rearrange_paths(lzr_optimizer* opt)
{
    for(size_t i = 0; i < paths->length; i++)
    {
        opt_path_t* current = (opt->paths + i);

        path_descriptor next = find_next(opt, i);
        swap_paths(current, next.path);

        if(next.invert)
            invert_path(current);

        //update the laser's current location
        opt->last_known_point = opt->points[current->b];
    }
}
