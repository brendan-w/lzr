
#include <string>
#include <timeline.h>



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
        void seek(lzr_time t);
    private:
        bool loaded = false;
        std::string path = "";
        lzr_time time;
        Timeline* timeline;
};
