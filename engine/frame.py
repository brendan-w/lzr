
#include <vector>
#include <lzr.h>


class Frame
{
    public:
        Frame();
        size_t size() { return points.size(); }

    private:
        std::vector<lzr_point> points;
};
