
#include "utils.h"
#include <math.h>

//constrains the given point to the area of LZR's frame
QPointF constrain_to_frame(QPointF p)
{
    p.setX(qMin(frame_bounds.right(), qMax(p.x(), frame_bounds.left())));
    p.setY(qMin(frame_bounds.bottom(), qMax(p.y(), frame_bounds.top())));
    return p;
}

float single_axis_snap(float x, int divisions)
{
    float division_size = (float) 2 / divisions;
    float remainder = fmod(x, division_size);
    float norm_pos = remainder / division_size;

    if(fabs(norm_pos) < 0.5)
    {
        //the value is on the inside of the halfway mark
        return x - remainder;
    }
    else if(norm_pos >= 0.5)
    {
        //outside of the halfway mark, positive quadrant
        return x + (division_size - remainder);
    }
    else if(norm_pos <= -0.5)
    {
        //outside of the halfway mark, negative quadrant
        return x - (division_size + remainder);
    }

    return 0; //else, something went wrong...
}

QPointF snap_to_grid(const QPointF& point, int divisions)
{
    return QPointF(single_axis_snap(point.x(), divisions),
                   single_axis_snap(point.y(), divisions));
}

QPointF constrain_and_snap(QPointF p, bool snap, int divisions)
{
    p = constrain_to_frame(p);
    if(snap)
        p = snap_to_grid(p, divisions);
    return p;
}
