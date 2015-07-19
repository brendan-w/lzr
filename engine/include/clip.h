
#include <vector>
#include <frame.h>


class Clip
{
    public:
        Clip();
        Frame* render(TimeNorm t);
    private:
        std::vector<Frame*> frames;
};
