
#include "etherdream.h"


EtherDream::EtherDream(std::string name) : DAC(name), dac(NULL)
{
    name = name.substr(sizeof(PREFIX_ETHERDREAM) - 1);
    unsigned long id = strtoul(name.c_str(), NULL, 16);
}

EtherDream::~EtherDream()
{
    
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

int EtherDream::write(Frame frame)
{
    return 0;
}

int EtherDream::stop()
{
    return 0;
}

int EtherDream::set_pps(int _pps)
{
    pps = _pps;
}

