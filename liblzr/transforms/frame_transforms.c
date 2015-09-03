

#include <math.h>
#include <lzr.h>


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
