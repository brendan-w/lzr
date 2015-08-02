
#include <timeline.h>


TimelineClip::TimelineClip()
{

}

TimelineClip::~TimelineClip()
{

}

lzr_frame TimelineClip::render(lzr_time t)
{
    //teh frame buffer used during rendering
    lzr_frame frame;
    frame.n_points = 0;

    //if the requested time falls within this clip
    if((start >= t) && (t <= stop))
    {
        //normalize the timecode, according to the clip's start and stop times
        t = (t - start) / (stop - start);
        //render the clip
        clip->render(&frame, t);
    }

    return frame;
}

void TimelineClip::set_clip(Clip* c)
{
    clip = c;
}

void TimelineClip::set_start(Time t)
{
    start = t;
}

void TimelineClip::set_stop(Time t)
{
    stop = t;
}
