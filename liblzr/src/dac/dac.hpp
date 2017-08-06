
#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <liblzr.hpp>

#define CLAMP(d) ( fmin(fmax(d, -1.0), 1.0) )

// DAC specific prefixes
#define PREFIX_ETHERDREAM "etherdream-"


namespace lzr {

class DAC_Internals
{
public:
    DAC_Internals();

    std::string name;
    bool connected;
    int pps;
};

} // namespace lzr
