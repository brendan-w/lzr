

#include <math.h>
#include <lzr.h>



//off-the-shelf ray casting test
static bool point_in_polygon(lzr_point p, lzr_frame* polygon)
{
    bool in = false;
    int i = 0;
    int j = 0;

    for(i = 0, j = polygon->n_points - 1; i < polygon->n_points; j = i++)
    {
        lzr_point a = polygon->points[i];
        lzr_point b = polygon->points[j];

        if( ((a.y > p.y) != (b.y > p.y)) &&
            (p.x < (b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x) )
            in = !in;
    }

    return in;
}


// http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/1968345#1968345
// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines 
// intersect the intersection point may be stored in lzr_point* output.
static int get_line_intersection(lzr_point a1, lzr_point a2, lzr_point b1, lzr_point b2, lzr_point* output)
{
    lzr_point s1, s2;

    s1.x = a2.x - a1.x;
    s1.y = a2.y - a1.y;
    
    s2.x = b2.x - b1.x;
    s2.y = b2.y - b1.y;

    float s, t;
    s = (-s1.y * (a1.x - b1.x) + s1.x * (a1.y - b1.y)) / (-s2.x * s1.y + s1.x * s2.y);
    t = ( s2.x * (a1.y - b1.y) - s2.y * (a1.x - b1.x)) / (-s2.x * s1.y + s1.x * s2.y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if(output != NULL)
        {
            output->x = a1.x + (t * s1.x);
            output->y = a1.y + (t * s1.y);
        }
        return 1;
    }

    return 0; // No collision
}


//safe point adding routine for frames
static int add_point(lzr_frame* frame, lzr_point p)
{
    //if the frame is full, produce error
    if(frame->n_points == LZR_FRAME_MAX_POINTS)
        return LZR_ERROR_TOO_MANY_POINTS;

    frame->points[frame->n_points] = p;
    frame->n_points++;
    return LZR_SUCCESS;
}

int lzr_frame_mask(lzr_frame* frame, lzr_frame* mask)
{
    //bail early if there aren't any mask points
    if(!mask->n_points)
        return LZR_SUCCESS;

    //setup a buffer to build the finished product
    lzr_frame output;
    output.n_points = 0;

    //eeewww
    //TODO: find a faster way to do this
    for(size_t i = 1; i < frame->n_points; i++)
    {
        for(size_t m = 1; i < mask->n_points; i++)
        {
            lzr_point intersection;
            if(get_line_intersection(frame->points[i - 1],
                                     frame->points[i],
                                     mask->points[i - 1],
                                     mask->points[i],
                                     &intersection))
            {
                //TODO
            }
        }
    }

    return LZR_SUCCESS;
}
