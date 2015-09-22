

#include <math.h>
#include <lzr.h>

/*
 * Simple Modifiers
 */

int lzr_frame_rotate(lzr_frame* frame, lzr_point axis, double theta)
{
    // x' = cos(theta) * (x-ox) - sin(theta) * (y-oy) + ox
    // y' = sin(theta) * (x-ox) + cos(theta) * (y-oy) + oy
    // where (ox, oy) is the axis/origin to rotate around

    //cache the sin and cos values
    double s = sin(theta);
    double c = cos(theta);

    for(size_t i = 0; i < frame->n_points; i++)
    {
        double x = frame->points[i].x - axis.x;
        double y = frame->points[i].y - axis.y;
        frame->points[i].x = c*x - s*y + axis.x;
        frame->points[i].y = s*x + c*y + axis.y;
    }

    return LZR_SUCCESS;
}

int lzr_frame_translate(lzr_frame* frame, lzr_point offset)
{
    for(size_t i = 0; i < frame->n_points; i++)
    {
        frame->points[i].x += offset.x;
        frame->points[i].y += offset.y;
    }

    return LZR_SUCCESS;
}

int lzr_frame_scale(lzr_frame* frame, double x, double y)
{
    for(size_t i = 0; i < frame->n_points; i++)
    {
        frame->points[i].x *= x;
        frame->points[i].y *= y;
    }

    return LZR_SUCCESS;
}

static lzr_point get_bounding_box_center(lzr_frame* frame)
{
    //edges
    double left = 1.0; //start these at opposite extremes
    double right = -1.0;
    double bottom = 1.0;
    double top = -1.0;

    for(size_t i = 0; i < frame->n_points; i++)
    {
        lzr_point p = frame->points[i];
        if(p.x < left)   left = p.x;
        if(p.x > right)  right = p.x;
        if(p.y < bottom) bottom = p.y;
        if(p.y > top)    top = p.y;
    }

    lzr_point center;
    center.x = (left + right) / 2.0;
    center.y = (bottom + top) / 2.0;

    return center;
}

static lzr_point get_average_center(lzr_frame* frame)
{
    lzr_point center;
    center.x = 0.0;
    center.y = 0.0;

    for(size_t i = 0; i < frame->n_points; i++)
    {
        lzr_point p = frame->points[i];
        center.x += p.x;
        center.y += p.y;
    }

    center.x /= (double) frame->n_points;
    center.y /= (double) frame->n_points;

    return center;
}

int lzr_frame_move_to(lzr_frame* frame, lzr_point position, int method)
{
    lzr_point center;

    switch(method)
    {
        case LZR_BOUNDING_BOX:
            center = get_bounding_box_center(frame);
            break;
        case LZR_AVERAGE:
            center = get_average_center(frame);
            break;
    }

    lzr_point offset;
    offset.x = position.x - center.x;
    offset.y = position.y - center.y;

    lzr_frame_translate(frame, offset);

    return LZR_SUCCESS;
}


/*
 * Duplicate Creators
 */

int lzr_frame_dup_linear(lzr_frame* frame, lzr_point offset, size_t n_dups, bool blank)
{
    //TODO: handle blanking jumps
    //check the point limits before any modifications are made
    if(frame->n_points * n_dups > LZR_FRAME_MAX_POINTS)
        return LZR_ERROR_TOO_MANY_POINTS;

    //mark the section of original points
    size_t orig = frame->n_points;

    for(size_t i = 1; i < n_dups; i++)
    {
        //TODO: generate blanking jump

        //duplicate the frame
        for(size_t p = 0; p < orig; p++)
        {
            lzr_point start = frame->points[p];
            lzr_point end = start;
            end.x += offset.x;
            end.y += offset.y;

            double t = (double) i / (n_dups - 1);

            //set the new point
            frame->points[frame->n_points] = lzr_point_lerp(&start, &end, t);
            frame->n_points++;
        }
    }

    return LZR_SUCCESS;
}
