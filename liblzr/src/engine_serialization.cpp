
#include <engine.hpp>

namespace lzr {

// show files
static const char* TIMELINE = "/timeline.json";
static const char* CLIPS_DIR = "clips";

// clip files
static const char* CLIP_EFFECTS = "/effects.json";


// Generator names
const char* Constant::name = "Constant";
const char* Linear::name   = "Linear";
const char* Curve::name    = "Curve";

// Effect names
const char* FrameEffect::name     = "FrameEffect";
const char* TranslateEffect::name = "TranslateEffect";



/*
 * Helper to save and load values based on whether they're
 * constant, or have been mapped to inputs. Assumes that
 * a json object "j", and "input_map" are present in the scope.
 */
#define SAVE(variable) {                                  \
    if(input_map.find(&(variable)) != input_map.end())    \
        j[#variable] = input_map[&(variable)];            \
    else                                                  \
        j[#variable] = (variable);                        \
}

#define LOAD(variable) {                        \
    if(j[#variable].is_string())                \
        input_map[&variable] = j[#variable];    \
    else                                        \
        (variable) = j[#variable];              \
}


/*
 * Helpers to instantiate Generators based on their names
 */

static Generator<double>* make_double_generator(const std::string& name)
{
    if(name == Constant::name)
        return new Constant();
    if(name == Linear::name)
        return new Linear();
    if(name == Curve::name)
        return new Curve();
    return nullptr;
}


//
// Constants
//

json Constant::serialize()
{
    json j;
    SAVE(x);
    return j;
}

void Constant::unserialize(const json& j)
{
    LOAD(x);
}

//
// Linear
//

json Linear::serialize()
{
    json j;
    SAVE(x);
    SAVE(from_a);
    SAVE(from_b);
    SAVE(to_a);
    SAVE(to_b);
    return j;
}

void Linear::unserialize(const json& j)
{
    LOAD(x);
    LOAD(from_a);
    LOAD(from_b);
    LOAD(to_a);
    LOAD(to_b);
}

//
// Curves
//

json Curve::serialize()
{
    json j;
    SAVE(x);
    j["generator"] = name;
    for(size_t i = 0; i < points.size(); i++)
    {
        j["points"][i] = points[i]->serialize(input_map);
    }
    return j;
}

void Curve::unserialize(const json& j)
{
    LOAD(x);

}

json Curve::CurvePoint::serialize(InputMap& input_map)
{
    json j;
    SAVE(x);
    SAVE(y);
    SAVE(lx);
    SAVE(ly);
    SAVE(rx);
    SAVE(ry);
    return j;
}

void Curve::CurvePoint::unserialize(json& j, InputMap& input_map)
{
    LOAD(x);
    LOAD(y);
    LOAD(lx);
    LOAD(ly);
    LOAD(rx);
    LOAD(ry);
}

//
// Effects
//


json FrameEffect::serialize()
{
    json j;
    j["type"] = name;
    j["n"] = n->serialize();
    return j;
}

void FrameEffect::unserialize(const json& j)
{
    {
        n = make_double_generator(j["n"]["generator"]);
    }
}


json TranslateEffect::serialize()
{
    json j;
    j["type"] = name;
    j["x"] = x->serialize();
    j["y"] = y->serialize();
    return j;
}

void TranslateEffect::unserialize(const json& j)
{
    (void) j;
}



//
// Clips
//

void Clip::save(std::string show)
{
    {
        std::ofstream f(show + "/" + CLIPS_DIR + "/" + name + "/" + CLIP_EFFECTS);
        json j;
        for(size_t i = 0; i < effects.size(); i++)
        {
            j[i] = effects[i]->serialize();
        }
        f << std::setw(4) << j << std::endl;
    }
}

void Clip::load(std::string show)
{
    {
        std::ifstream f(show + "/" + CLIPS_DIR + "/" + name + "/" +  + CLIP_EFFECTS);
        json j;
        f >> j;

        for(json e : j)
        {

        }
    }
}


//
// Shows
//

void Show::save(std::string show)
{
    {
        std::ofstream f(show + TIMELINE);
        f << std::setw(4) << serialize_timeline() << std::endl;
    }
}

void Show::load(std::string show)
{
    {
        std::ifstream f(show + TIMELINE);
        json j;
        f >> j;
        unserialize_timeline(j);
    }
}

json Show::serialize_timeline()
{
    json j;
    for(size_t i = 0; i < timeline.size(); i++)
    {
        TimelineClip& t_clip = timeline[i];
        j[i]["clip"] = t_clip.clip->name;
        j[i]["start"] = t_clip.start;
        j[i]["end"] = t_clip.end;
    }
    return j;
}

void Show::unserialize_timeline(json& j)
{
    // load clips
    for(size_t i = 0; i < j.size(); i++)
    {
        TimelineClip t_clip;
        std::string name = j[i]["clip"];
        t_clip.start     = j[i]["start"];
        t_clip.end       = j[i]["end"];
        t_clip.clip      = get_clip_by_name(name);
        timeline.push_back(t_clip);
    }
}

Clip* Show::get_clip_by_name(const std::string& name)
{
    // TODO: make this not horrible
    for(Clip* clip : clips)
    {
        if(clip->name == name)
            return clip;
    }

    return nullptr;
}


} // namespace lzr