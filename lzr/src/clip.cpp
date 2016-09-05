

#include "clip.h"


Clip::Clip()
{
    //new clips are always populated with a base frame generator effect
    effects.push_back(new FrameEffect());
}

Clip::~Clip()
{
    for(Effect* e : effects)
    {
        delete e;
    }
}

lzr::Frame Clip::run(Time& t) {
    lzr::Frame frame;

    for(Effect* e : effects)
    {
        if(e->active)
            e->run(frame, t);
    }

    return frame;
}
