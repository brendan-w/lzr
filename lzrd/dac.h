
#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <math.h>
#include <liblzr.h>

using namespace lzr;

#define CLAMP(d) ( fmin(fmax(d, -1.0), 1.0) )

#define PREFIX_ETHERDREAM "EtherDream-"

typedef std::vector<std::string> DACList;


class DAC
{
public:
    DAC(std::string name);
    virtual ~DAC();

    //standard DAC interface
    std::string name();
    virtual int send(Frame frame) = 0;
    virtual int stop() = 0;
    virtual int set_pps(int pps) = 0;

protected:
    std::string _name;
};

//main DAC handling functions
void init_dacs();
DACList list_dacs();
DAC* dac_connect(std::string name);
