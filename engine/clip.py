
#include <vector>
#include "frame.h"


class Clip
{
    public:
        Clip();
        Frame* time_to_frame(TimeNorm t);
    private:
        std::vector<Frame*> frames;
};
