

#include "clip.h"


lzr::Frame Clip::run(Time& t) {
    lzr::Frame frame;

    for(Effect* e : effects)
    {
        if(e->active)
            e->run(frame, t);
    }

    return frame;
}
