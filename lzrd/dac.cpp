
#include "dac.h"
#include "etherdream.h"

void init_dacs()
{
    etherdream_lib_start();
}

DACList list_dacs()
{
    return EtherDream::list_dacs();
}


static inline bool starts_with(const std::string& str,
                               const std::string& substr)
{
    return str.compare(0, substr.length(), substr) == 0;
}

DAC* dac_connect(std::string name)
{
    if(starts_with(name, PREFIX_ETHERDREAM))
    {
        return new EtherDream(name);
    }

    return NULL;
}



DAC::DAC(std::string name) : _name(name)
{
    
}

std::string DAC::name()
{
    return _name;
}
