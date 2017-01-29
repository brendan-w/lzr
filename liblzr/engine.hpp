
#pragma once

#include <map>
#include <vector>
#include <fstream>

#include <liblzr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
void read_inputs(Inputs& inputs, InputMap& map);

/******************************************************************************/
/*  Generators                                                                */
/******************************************************************************/

template <typename T>
struct Generator
{
    virtual T operator()(Inputs& inputs)=0;
    virtual json serialize()=0;
    virtual void unserialize(const json& j)=0;
    InputMap input_map;
};

//
// Constant
//

struct Constant : public Generator<double>
{
    static const char* name;

    double operator()(Inputs& inputs) { (void) inputs; return x; };
    double x;

    json serialize();
    void unserialize(const json& j);
};

//
// Linear map
//

struct Linear : public Generator<double>
{
    static const char* name;

    double operator()(Inputs& inputs)
    {
        read_inputs(inputs, input_map);
        return (x - from_a) * (to_b - to_a) / (from_b - from_a) + to_a;
    };
    double x;
    double from_a;
    double from_b;
    double to_a;
    double to_b;

    json serialize();
    void unserialize(const json& j);
};

//
// Curve
//

class Curve : public Generator<double>
{
public:
    static const char* name;

    struct CurvePoint
    {
        double x;
        double y;
        double lx;
        double ly;
        double rx;
        double ry;

        json serialize(InputMap& input_map);
        void unserialize(json& j, InputMap& input_map);
    };

    ~Curve();

    double operator()(Inputs& inputs);

    json serialize();
    void unserialize(const json& j);

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
    virtual json serialize()=0;
    virtual void unserialize(const json& j)=0;
};

struct FrameEffect : public Effect
{
    static const char* name;

    FrameList frames;
    Generator<double>* n;

    void operator()(Frame& frame, Inputs& inputs)
    {
        // TODO: clamp to prevent segfaults
        frame = frames[(int)(*n)(inputs)];
    };

    json serialize();
    void unserialize(const json& j);
};

struct TranslateEffect : public Effect
{
    static const char* name;

    Generator<double>* x;
    Generator<double>* y;

    void operator()(Frame& frame, Inputs& inputs)
    {
        translate(frame, (*x)(inputs), (*y)(inputs));
    };

    json serialize();
    void unserialize(const json& j);
};


/******************************************************************************/
/*  Clips                                                                     */
/******************************************************************************/

struct Clip
{
    Clip(std::string name_) : name(name_) {};

    Frame operator()(Inputs& inputs)
    {
        Frame frame;
        for(Effect* effect : effects)
        {
            (*effect)(frame, inputs);
        }
        return frame;
    };

    void save(std::string show);
    void load(std::string show);

    std::vector<Effect*> effects;
    std::string name;
};


/******************************************************************************/
/*  Master Timeline                                                           */
/******************************************************************************/


class Show
{
public:
    struct TimelineClip
    {
        Clip* clip;
        double start;
        double end;
    };

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

    void save(std::string show);
    void load(std::string show);

    std::vector<Clip*> clips;
    std::vector<TimelineClip> timeline;

private:
    Clip* get_clip_by_name(const std::string& name);

    json serialize_timeline();
    void unserialize_timeline(json& j);
};


} // namespace lzr
