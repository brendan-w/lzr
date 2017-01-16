
#include <engine.hpp>
#include <algorithm> // std::sort()

namespace lzr {


Curve::Curve()
{

}

Curve::~Curve()
{

}

double Curve::operator()(Inputs& inputs)
{
    read_inputs(inputs, input_map);
    return compute();
}

CurvePoint* Curve::add(double x_)
{
    CurvePoint* point = new CurvePoint();
    point->x = x_;
    points.push_back(point);

    // put the points back in ascending order
    std::sort(points.begin(), points.end(), [](const CurvePoint* a, const CurvePoint* b) -> bool
    { 
        return a->x > b->x;
    });

    return point;
}

void Curve::remove(CurvePoint* point)
{

}

void Curve::clear()
{
    
}

double Curve::compute()
{
    if(points.size() == 0)
        return 0.0;

    if(points.size() == 1)
        return points[0]->y;

    if(x <= 0.0)
        return points.front()->y;

    if(x >= 1.0)
        return points.back()->y;

    // if we've come this far, it means we need to compute a curve

    // find the points to our left and right
    // TODO: binary search
    CurvePoint* left_point;
    CurvePoint* right_point;
    for(CurvePoint* point : points)
    {
        if(x < point->x)
        {
            right_point = point;
            break;
        }
        else
        {
            left_point = point;                
        }
    }

    // TODO: perform the bezier math
    return left_point->y;
}

} // namespace lzr
