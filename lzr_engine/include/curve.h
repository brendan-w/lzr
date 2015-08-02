
#include <vector>
#include <lzr.h>

typedef struct {
    lzr_time t;
    double v;
} curve_point_t;

class Curve
{
    public:
        Curve();
        ~Curve();
        double render(lzr_time nt);
    private:
        std::vector<curve_point_t> points;
};
