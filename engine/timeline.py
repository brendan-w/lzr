
#include <vector>
#include "clip.h"


class TimelineClip
{
    public:
        TimelineClip();
        Frame* time_to_frame(Time t);
        void set_clip(Clip* c);
        void set_zone(Zone* z);
        void set_start(Time t);
        void set_stop(Time t);

    private:
        Clip* clip;
        Zone* zone;
        Time start;
        Time stop;        
};

class Timeline
{
    public:
        Timeline();
        Frame* time_to_frame(Time t);
        void add_clip(TimelineClip* c);
        void remove_clip(TimelineClip* c);
    private:
        std::vector<TimelineClip*> clips;

};
