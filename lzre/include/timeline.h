
#include <vector>
#include <clip.h>
#include <lzr.h>



class TimelineClip
{
    public:
        TimelineClip();
        virtual ~TimelineClip();
        lzr_frame render(lzr_time t);
        void set_clip(Clip* c);
        void set_start(lzr_time t);
        void set_stop(lzr_time t);

    private:
        Clip* clip;
        lzr_time start;
        lzr_time stop;        
};

class Timeline
{
    public:
        Timeline();
        virtual ~Timeline();
        lzr_frame render(lzr_time t);
        void add_clip(TimelineClip* c);
        void remove_clip(TimelineClip* c);
    private:
        std::vector<TimelineClip*> clips;
};
