
#include <engine.hpp>
#include <algorithm> // std::sort(), std::remove()

namespace lzr {

// Generator names
const char* Constant::name = "Constant";
const char* Linear::name   = "Linear";
const char* Curve::name    = "Curve";

// Effect names
const char* FrameEffect::name     = "FrameEffect";
const char* TranslateEffect::name = "TranslateEffect";
const char* RotateEffect::name = "RotateEffect";


Curve::~Curve()
{
    clear();
}

double Curve::operator()(Inputs& inputs)
{
    read_inputs(inputs, input_map);
    return compute();
}

Curve::CurvePoint* Curve::add(double x_)
{
    Curve::CurvePoint* point = new CurvePoint();
    point->x = x_;
    points.push_back(point);

    // put the points back in ascending order
    std::sort(points.begin(), points.end(), [](const Curve::CurvePoint* a, const Curve::CurvePoint* b) -> bool
    { 
        return a->x > b->x;
    });

    return point;
}

void Curve::remove(CurvePoint* point)
{
    // detach any inputs
    input_map.erase(&(point->x));
    input_map.erase(&(point->y));
    input_map.erase(&(point->lx));
    input_map.erase(&(point->ly));
    input_map.erase(&(point->rx));
    input_map.erase(&(point->ry));

    // remove the point
    delete point;
    points.erase(std::remove(points.begin(), points.end(), point));
}

void Curve::clear()
{
    // TODO make not hacky
    while(points.size() > 0)
    {
        remove(points[0]);
    }
}

double Curve::compute()
{
    if(points.size() == 0)
        return 0.0;

    if(points.size() == 1)
        return points[0]->y;

    if(v <= 0.0)
        return points.front()->y;

    if(v >= 1.0)
        return points.back()->y;

    // if we've come this far, it means we need to compute a curve

    // find the points to our left and right
    // TODO: binary search
    Curve::CurvePoint* left_point;
    Curve::CurvePoint* right_point;
    for(CurvePoint* point : points)
    {
        if(v < point->x)
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
    (void) right_point;
    return left_point->y;
}


//
// Clips
//

Frame Clip::operator()(Inputs& inputs)
{
    Frame frame;
    for(Effect* effect : effects)
    {
        (*effect)(frame, inputs);
    }
    return frame;
}

//
// Shows
//

Show::~Show()
{
    for(auto it : clips)
        delete it.second;
}

Frame Show::operator()(double time)
{
    Inputs inputs; // empty set of inputs
    return operator()(time, inputs);
}

Frame Show::operator()(double time, Inputs& inputs)
{
    Frame frame;

    // TODO: make not slow and naive
    for(TimelineClip& t_clip : timeline)
    {
        // if we're in this clip
        if((time >= t_clip.start) &&
           (time <= t_clip.end) &&
           (t_clip.clip != nullptr))
        {
            // execute this clip at the given time
            inputs["time"] = time;
            inputs["clip_time"] = (time - t_clip.start) / (t_clip.end - t_clip.start);
            frame.add_with_blank_jump( (*(t_clip.clip))(inputs) );
        }
    }

    return frame;
}

} // namespace lzr
