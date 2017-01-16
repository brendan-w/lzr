
#pragma once

#include <map>
#include <vector>

#include <liblzr.h>

namespace lzr {

/******************************************************************************/
/*  Input Handling                                                            */
/******************************************************************************/

/*
 * This struct contains *all* of the varying input signals to a clip.
 * This covers deterministic variables like time, as well as realtime
 * signals such as named DMX and MIDI values.
 *
 * For instance:
 * "time"
 * "clip-time"
 * "DMX Channel 2 (size)"
 */
typedef std::map<std::string, double> Inputs;

/*
 * A mapping of named inputs to their final locations.
 * This is meant to be maintained at the effect level
 * (ie, each effect can have it's own mapping).
 */
typedef std::map<double*, std::string> InputMap;

/*
 * Helper function for loading a new set of inputs
 * into their final destinations (as specified by the InputMap)
 */
void read_inputs(Inputs& inputs, InputMap& map)
{
    for(auto it : map)
        *(it.first) = inputs[it.second];
}


/******************************************************************************/
/*  Generators                                                                */
/******************************************************************************/

template <typename T>
struct Generator
{
    virtual T operator()(Inputs& inputs)=0;
    InputMap input_map;
};

//
// Constant
//

struct Constant : public Generator<double>
{
    double v;
    double operator()(Inputs& inputs) { (void) inputs; return v; };
};

//
// Linear map
//

struct Linear : public Generator<double>
{

};

//
// Oscillators
//

struct SinOscillator : public Generator<double>
{

};

//
// Curve
//

struct CurvePoint
{
    double x;
    double y;
    double lx;
    double ly;
    double rx;
    double ry;
};

class Curve : public Generator<double>
{
public:
    Curve();
    ~Curve();

    double operator()(Inputs& inputs);

    CurvePoint* add(double x_);
    void remove(CurvePoint* point);
    void clear();

    // target for mapping the incoming "time" value for this curve
    // though it need not be set to "time"
    double x;
    std::vector<CurvePoint*> points;

private:
    double compute();
};



/******************************************************************************/
/*  Effects                                                                   */
/******************************************************************************/

struct Effect
{
    virtual void operator()(Frame& frame, Inputs& inputs)=0;
};

struct FrameEffect : public Effect
{
    FrameList frames;
    Generator<double>* n;

    void operator()(Frame& frame, Inputs& inputs)
    {
        // TODO: clamp to prevent segfaults
        frame = frames[(int)(*n)(inputs)];
    };
};

struct TranslateEffect : public Effect
{
    Generator<double>* x;
    Generator<double>* y;

    void operator()(Frame& frame, Inputs& inputs)
    {
        translate(frame, (*x)(inputs), (*y)(inputs));
    };
};


/******************************************************************************/
/*  Clips                                                                     */
/******************************************************************************/

struct Clip
{
    Frame operator()(Inputs& inputs)
    {
        Frame frame;
        for(Effect* effect : effects)
        {
            (*effect)(frame, inputs);
        }
        return frame;
    };

    std::vector<Effect*> effects;
};


/******************************************************************************/
/*  Master Timeline                                                           */
/******************************************************************************/

struct TimelineClip
{
    Clip* clip;
    double start;
    double end;
};

class Show
{
    ~Show()
    {
        for(Clip* clip : clips)
            delete clip;
    }

    Frame operator()(double time, Inputs& inputs)
    {
        Frame frame;

        // TODO: make not slow and naive
        for(TimelineClip& t_clip : timeline)
        {
            // if we're in this clip
            if((time > t_clip.start) && (time < t_clip.end))
            {
                // execute this clip at the given time
                inputs["time"] = time;
                inputs["clip_time"] = (time - t_clip.start) / (t_clip.end - t_clip.start);
                frame.add_with_blank_jump( (*(t_clip.clip))(inputs) );
            }
        }

        return frame;
    }

    std::vector<Clip*> clips;
    std::vector<TimelineClip> timeline;
};


} // namespace lzr
