

#include <math.h>
#include <lzr.h>

/*
 * in-place functions that don't alter the number of points
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

int lzr_frame_move_to(lzr_frame* frame, lzr_point new_center, int method)
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
    offset.x = new_center.x - center.x;
    offset.y = new_center.y - center.y;

    lzr_frame_translate(frame, offset);

    return LZR_SUCCESS;
}


/*
 * functions increase the number of points in a frame
 */

int lzr_frame_combine(lzr_frame* a, lzr_frame* b, bool blank)
{
    //bail, if there's nothing to do (avoids adding a useless blanking jump)
    if(b->n_points == 0)
        return LZR_SUCCESS;

    //check for an overflow
    size_t total = a->n_points + b->n_points + (blank ? 1 : 0);
    if(total > LZR_FRAME_MAX_POINTS)
        return LZR_ERROR_TOO_MANY_POINTS;

    //avoids a bunch of derefs, plus, it's prettier
    size_t n = a->n_points;

    if(blank)
    {
        //create the blanking jump to the start of the B frame
        a->points[n] = b->points[0];
        LZR_POINT_BLANK(a->points[n]);
        n++;
    }

    for(size_t i = 0; i < b->n_points; i++)
    {
        a->points[n] = b->points[i];
        n++;
    }

    a->n_points = n;

    return LZR_SUCCESS;
}

int lzr_frame_mirror(lzr_frame* frame)
{
    return LZR_SUCCESS;
}

int lzr_frame_dup_linear(lzr_frame* frame, lzr_point offset, size_t n_dups, bool blank)
{
    //TODO: decide
    //n_dups--; //give a visually correct readout of the number

    //bail, if there's nothing to do
    if((frame->n_points == 0) || (n_dups == 0))
        return LZR_SUCCESS;

    //check for an overflow
    size_t total = (frame->n_points * (n_dups + 1)) + (blank ? n_dups : 0);
    if(total > LZR_FRAME_MAX_POINTS)
        return LZR_ERROR_TOO_MANY_POINTS;

    lzr_frame orig = *frame;

    //compute the offset for one duplication
    offset.x /= (double) n_dups;
    offset.y /= (double) n_dups;

    for(size_t i = 0; i < n_dups; i++)
    {
        lzr_frame_translate(&orig, offset);
        int r = lzr_frame_combine(frame, &orig, blank);
        if(r != LZR_SUCCESS)
            return r;
    }

    return LZR_SUCCESS;
}

int lzr_frame_dup_radial(lzr_frame* frame, lzr_point axis, size_t n_dups, double angle, bool blank)
{
    //TODO: decide
    //n_dups--; //give a visually correct readout of the number

    //bail, if there's nothing to do
    if((frame->n_points == 0) || (n_dups == 0))
        return LZR_SUCCESS;

    //check for an overflow
    size_t total = (frame->n_points * (n_dups + 1)) + (blank ? n_dups : 0);
    if(total > LZR_FRAME_MAX_POINTS)
        return LZR_ERROR_TOO_MANY_POINTS;

    lzr_frame orig = *frame;

    //compute the angular offset for one duplication
    angle /= n_dups;

    for(size_t i = 0; i < n_dups; i++)
    {
        lzr_frame_rotate(&orig, axis, angle);
        int r = lzr_frame_combine(frame, &orig, blank);
        if(r != LZR_SUCCESS)
            return r;
    }

    return LZR_SUCCESS;
}
