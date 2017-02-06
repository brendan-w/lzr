
#include <sys/stat.h> // mkdir
#include <errno.h>
#include <glob.h>
#include <engine.hpp>

namespace lzr {

/*
    Shows have the following directory structure:
    .
    ├── clips/
    │   └── Stars/
    │       ├── effects.json
    │       └── frames.ild
    ├── inputs.json
    └── timeline.json
*/

// 777, let the umask take care of this
static const mode_t DIR_PERMS = S_IRUSR | S_IWUSR | S_IXUSR | \
                                S_IRGRP | S_IWGRP | S_IXGRP | \
                                S_IROTH | S_IWOTH | S_IXOTH;

// show files
static const char* TIMELINE = "timeline.json";
static const char* CLIPS_DIR = "clips";

// clip files
static const char* CLIP_EFFECTS = "effects.json";



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
    if(name == RotateEffect::name)
        return new RotateEffect();
    return nullptr;
}

inline std::vector<std::string> glob(const std::string& pattern){
    std::vector<std::string> ret;

    glob_t glob_result;
    glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);

    for(size_t i = 0; i < glob_result.gl_pathc; i++)
    {
        ret.push_back(std::string(glob_result.gl_pathv[i]));
    }

    globfree(&glob_result);
    return ret;
}

//
// Constants
//

json Constant::serialize()
{
    json j;
    SAVE_INPUT(j, input_map, v);
    return j;
}

void Constant::unserialize(const json& j)
{
    LOAD_INPUT(j, input_map, v);
}

//
// Linear
//

json Linear::serialize()
{
    json j;
    SAVE_INPUT(j, input_map, v);
    SAVE_INPUT(j, input_map, from_a);
    SAVE_INPUT(j, input_map, from_b);
    SAVE_INPUT(j, input_map, to_a);
    SAVE_INPUT(j, input_map, to_b);
    return j;
}

void Linear::unserialize(const json& j)
{
    LOAD_INPUT(j, input_map, v);
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
    SAVE_INPUT(j, input_map, v);
    j["__generator__"] = name;
    for(size_t i = 0; i < points.size(); i++)
    {
        j["points"][i] = points[i]->serialize(input_map);
    }
    return j;
}

void Curve::unserialize(const json& j)
{
    LOAD_INPUT(j, input_map, v);
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

// Frame
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

// Translate
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

// Rotate

json RotateEffect::serialize()
{
    json j;
    j["__effect__"] = name;
    SAVE_GENERATOR(j, cx);
    SAVE_GENERATOR(j, cy);
    SAVE_GENERATOR(j, theta);
    return j;
}

void RotateEffect::unserialize(const json& j)
{
    LOAD_GENERATOR(j, cx);
    LOAD_GENERATOR(j, cy);
    LOAD_GENERATOR(j, theta);
}


//
// Clips
//

void Clip::save(std::string path)
{
    std::ofstream f(path + "/" + CLIP_EFFECTS);
    json j;
    for(size_t i = 0; i < effects.size(); i++)
    {
        j[i] = effects[i]->serialize();
    }
    f << std::setw(4) << j << std::endl;
}

void Clip::load(std::string path)
{
    std::ifstream f(path + "/" +  + CLIP_EFFECTS);
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
    mkdir((show + "/" + CLIPS_DIR).c_str(), DIR_PERMS);

    for(auto it : clips)
    {
        std::string name = it.first;
        Clip* clip = it.second;
        std::string path = show + "/" + CLIPS_DIR + "/" + name;
        mkdir(path.c_str(), DIR_PERMS);
        clip->save(path);
    }

    {
        std::ofstream f(show + "/" + TIMELINE);
        f << std::setw(4) << serialize_timeline() << std::endl;
    }
}

void Show::load(std::string show)
{
    {
        for(const std::string& name : glob(show + "/" + CLIPS_DIR + "/*"))
        {
            Clip* clip = new Clip(name);
            clip->load(show + "/" + CLIPS_DIR + "/" + name);
            clips[name] = clip;
        }
    }
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
        t_clip.clip      = get_clip_by_name(j[i]["clip"]);
        t_clip.start     = j[i]["start"];
        t_clip.end       = j[i]["end"];
        timeline.push_back(t_clip);
    }
}

Clip* Show::get_clip_by_name(const std::string& name)
{
    if(clips.find(name) != clips.end())
        return clips[name];
    return nullptr;
}


} // namespace lzr