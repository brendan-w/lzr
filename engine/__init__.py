
#include <string>
#include "timeline.h"

typedef double Time;     //seconds
typedef double TimeNorm; //[0,1]

class Lazer
{
    public:
        Lazer();
        void new_show(std::string filepath);
        void load_show(std::string filepath);
        void save_show(std::string filepath);
        void start();
        void stop();
        void pause();
        void seek(Time t);
    private:
        bool loaded = false;
        std::string path = "";
        Time time;
        Timeline* timeline;
};
