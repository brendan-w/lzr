
#include "dac.hpp"
#include "etherdream.hpp"

namespace lzr {


static inline bool starts_with(const std::string& str,
                               const std::string& substr)
{
    return str.compare(0, substr.length(), substr) == 0;
}

static inline void append_dacs(DACList& master, DACList other)
{
    master.insert(master.end(), other.begin(), other.end());
}

void init_dacs()
{
    etherdream_lib_start();
}

DACList list_dacs()
{
    DACList list;
    append_dacs(list, EtherDream::list_dacs());
    return list;
}

//factory for DAC connection objects
DAC* dac_connect(std::string name)
{
    DAC* dac = NULL;

    //instantiate the right connection class
    if(starts_with(name, PREFIX_ETHERDREAM))
    {
        dac = new EtherDream(name);
    }

    //check that we did something worth returning
    if(dac)
    {
        if(dac->connected())
            return dac; //success!
        else
            delete dac; //failed: abort
    }

    return NULL;
}



DAC_Internals::DAC_Internals() : name(""),
                                 pps(15000)
{

}


/*
 * DAC class functions
 */

DAC::DAC(std::string name)
{
    internal = new DAC_Internals();
    internal->name = name;
}

DAC::~DAC()
{
    delete internal;
}

std::string DAC::name()
{
    return internal->name;
}

bool DAC::connected()
{
    return internal->connected;
}

void DAC::pps(int pps)
{
    internal->pps = pps;
}

int DAC::pps()
{
    return internal->pps;
}


} // namespace lzr
