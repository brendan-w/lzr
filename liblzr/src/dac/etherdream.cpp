
#include "etherdream.hpp"

namespace lzr {


EtherDream::EtherDream(std::string name) : DAC(name),
                                           dac(NULL)
{
    name = name.substr(sizeof(PREFIX_ETHERDREAM) - 1);
    unsigned long id = strtoul(name.c_str(), NULL, 16);
    dac = etherdream_get(id);
    internal->connected = (etherdream_connect(dac) == 0);
}

EtherDream::~EtherDream()
{
    stop();
    etherdream_disconnect(dac);
}

DACList EtherDream::list_dacs()
{
    int total = etherdream_dac_count();
    DACList dacs(total);

    for(int i = 0; i < total; i++)
    {
        struct etherdream* dac = etherdream_get(i);
        unsigned long id = etherdream_get_id(dac);

        std::stringstream name;
        name << PREFIX_ETHERDREAM;
        name << std::setfill('0') << std::setw(6) << std::hex << id;

        dacs[i] = name.str();
    }

    return dacs;
}

int EtherDream::send(Frame frame)
{
    int r = 0;

    //if the laser is ready for another frame
    if(etherdream_is_ready(dac) == 1)
    {
        buffer.resize(frame.size());

        for(size_t i = 0; i < frame.size(); i++)
        {
            //convert LZR point into etherdream point
            Point& p = frame[i];
            struct etherdream_point ep;

            ep.x = (int16_t) (CLAMP(p.x) * 32767);
            ep.y = (int16_t) (CLAMP(p.y) * 32767);
            ep.r = (uint16_t) (p.r * 255);
            ep.g = (uint16_t) (p.g * 255);
            ep.b = (uint16_t) (p.b * 255);
            ep.i = (uint16_t) (p.i * 255);

            buffer[i] = ep;
        }

        r = etherdream_write(dac, buffer.data(), buffer.size(), internal->pps, -1);
    }
    //else, dump the frame, an old one is still being drawn
    //TODO: ^ is this really a good idea? Could create a stutterring animation

    return r;
}

int EtherDream::stop()
{
    return etherdream_stop(dac);
}


} // namespace lzr
