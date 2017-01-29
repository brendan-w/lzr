
#include <sys/stat.h> // mkdir
#include <glob.h>
#include <engine.hpp>

namespace lzr {

/*
    Shows have the following directory structure:
    .
    ├── clips/
    │   └── stars/
    │       ├── effects.json
    │       └── frames.ild
    ├── inputs.json
    └── timeline.json
*/

static const mode_t PERMS = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

// show files
static const char* TIMELINE = "timeline.json";
static const char* CLIPS_DIR = "clips";

// clip files
static const char* CLIP_EFFECTS = "effects.json";



// Generator names
const char* Constant::name = "Constant";
const char* Linear::name   = "Linear";
const char* Curve::name    = "Curve";

// Effect names
const char* FrameEffect::name     = "FrameEffect";
const char* TranslateEffect::name = "TranslateEffect";



/*
 * Helper to save and load values based on whether they're
 * constant, or have been mapped to inputs.
 */
#define SAVE_INPUT(json, input_map, variable) {           \
    if(input_map.find(&(variable)) != input_map.end())    \
        json[#variable] = input_map[&(variable)];         \
    else                                                  \
        json[#variable] = (variable);                     \
}

#define LOAD_INPUT(json, input_map, variable) {    \
    if(json[#variable].is_string())                \
        input_map[&variable] = json[#variable];    \
    else                                           \
        (variable) = json[#variable];              \
}

/*
 * Helper to save and load effect parameters (generators).
 * Not strictly neccessary, simply makes for concise code.
 */
#define SAVE_GENERATOR(json, variable) {          \
    json[#variable] = (variable)->serialize();    \
}

#define LOAD_GENERATOR(json, variable) {                                     \
    (variable) = make_double_generator(json[#variable]["__generator__"]);    \
    (variable)->unserialize(json[#variable]);                                \
}

/*
 * LUTs to instantiate Generators based on their names
 */

static Generator<double>* make_double_generator(const std::string& name)
{
    // TODO: hash? to make faster?
    if(name == Constant::name)
        return new Constant();
    if(name == Linear::name)
        return new Linear();
    if(name == Curve::name)
        return new Curve();
    return nullptr;
}

static Effect* make_effect(const std::string& name)
{
    if(name == FrameEffect::name)
        return new FrameEffect();
    if(name == TranslateEffect::name)
        return new TranslateEffect();
    return nullptr;
}

//
// Constants
//

json Constant::serialize()
{
    json j;
    SAVE_INPUT(j, input_map, x);
    return j;
}

void Constant::unserialize(const json& j)
{
    LOAD_INPUT(j, input_map, x);
}

//
// Linear
//

json Linear::serialize()
{
    json j;
    SAVE_INPUT(j, input_map, x);
    SAVE_INPUT(j, input_map, from_a);
    SAVE_INPUT(j, input_map, from_b);
    SAVE_INPUT(j, input_map, to_a);
    SAVE_INPUT(j, input_map, to_b);
    return j;
}

void Linear::unserialize(const json& j)
{
    LOAD_INPUT(j, input_map, x);
    LOAD_INPUT(j, input_map, from_a);
    LOAD_INPUT(j, input_map, from_b);
    LOAD_INPUT(j, input_map, to_a);
    LOAD_INPUT(j, input_map, to_b);
}

//
// Curves
//

json Curve::serialize()
{
    json j;
    SAVE_INPUT(j, input_map, x);
    j["__generator__"] = name;
    for(size_t i = 0; i < points.size(); i++)
    {
        j["points"][i] = points[i]->serialize(input_map);
    }
    return j;
}

void Curve::unserialize(const json& j)
{
    LOAD_INPUT(j, input_map, x);
    for(const json jp : j["points"])
    {
        CurvePoint* p = new CurvePoint();
        p->unserialize(jp, input_map);
        points.push_back(p);
    }
}

json Curve::CurvePoint::serialize(InputMap& input_map)
{
    json j;
    SAVE_INPUT(j, input_map, x);
    SAVE_INPUT(j, input_map, y);
    SAVE_INPUT(j, input_map, lx);
    SAVE_INPUT(j, input_map, ly);
    SAVE_INPUT(j, input_map, rx);
    SAVE_INPUT(j, input_map, ry);
    return j;
}

void Curve::CurvePoint::unserialize(const json& j, InputMap& input_map)
{
    LOAD_INPUT(j, input_map, x);
    LOAD_INPUT(j, input_map, y);
    LOAD_INPUT(j, input_map, lx);
    LOAD_INPUT(j, input_map, ly);
    LOAD_INPUT(j, input_map, rx);
    LOAD_INPUT(j, input_map, ry);
}

//
// Effects
//

json FrameEffect::serialize()
{
    json j;
    j["__effect__"] = name;
    SAVE_GENERATOR(j, n);
    return j;
}

void FrameEffect::unserialize(const json& j)
{
    LOAD_GENERATOR(j, n);
}


json TranslateEffect::serialize()
{
    json j;
    j["__effect__"] = name;
    SAVE_GENERATOR(j, x);
    SAVE_GENERATOR(j, y);
    return j;
}

void TranslateEffect::unserialize(const json& j)
{
    LOAD_GENERATOR(j, x);
    LOAD_GENERATOR(j, y);
}



//
// Clips
//

void Clip::save(std::string show)
{
    std::ofstream f(show + "/" + CLIPS_DIR + "/" + name + "/" + CLIP_EFFECTS);
    json j;
    for(size_t i = 0; i < effects.size(); i++)
    {
        j[i] = effects[i]->serialize();
    }
    f << std::setw(4) << j << std::endl;
}

void Clip::load(std::string show)
{
    std::ifstream f(show + "/" + CLIPS_DIR + "/" + name + "/" +  + CLIP_EFFECTS);
    json j;
    f >> j;

    for(const json je : j)
    {
        Effect* effect = make_effect(je["__effect__"]);
        effect->unserialize(je);
        effects.push_back(effect);
    }
}


//
// Shows
//

void Show::save(std::string show)
{
    int r = mkdir((show + "/" + CLIPS_DIR).c_str(), PERMS);

    {
        for(Clip* clip : clips)
        {
            r = mkdir((show + "/" + CLIPS_DIR + "/" + clip->name).c_str(), PERMS);
            clip->save(show);
        }
    }

    {
        std::ofstream f(show + "/" + TIMELINE);
        f << std::setw(4) << serialize_timeline() << std::endl;
    }
}

void Show::load(std::string show)
{
    {
        std::ifstream f(show + "/" + TIMELINE);
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