
#include <clip.h>

Clip::Clip()
{

}

Clip::~Clip()
{

}

void Clip::render(lzr_frame* frame, lzr_time nt)
{
    //stupid, single-frame rendering for now
    *frame = base_frame;
}
