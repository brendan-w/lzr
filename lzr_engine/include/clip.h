

#include <vector>
#include <effect.h>
#include <lzr.h>


class Clip
{
    public:
        Clip();
        virtual ~Clip();
        void render(lzr_frame* frame, lzr_time nt);
    private:
        lzr_frame base_frame; //the root laser frame, on which the effects will operate
        std::vector<Effect*> effects; //the effects stack
        std::vector<Curve*> curves; //the clip's timeline curves
};
