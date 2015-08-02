
#include <clip.h>

Clip::Clip()
{

}

Clip::~Clip()
{

}

void Clip::render(lzr_frame* frame, lzr_time nt)
{
    //load the initial frame
    *frame = base_frame;

    //render the effects
    for(Effect* effect : effects)
        effect->render(frame, nt);
}
