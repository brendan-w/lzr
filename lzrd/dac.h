
#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <liblzr.h>
using namespace lzr;

#define PREFIX_ETHERDREAM "EtherDream-"

typedef std::vector<std::string> DACList;


class DAC
{
public:
    DAC(std::string name);

    //standard DAC interface
    std::string name();
    virtual int write(Frame frame) = 0;
    virtual int stop() = 0;
    virtual int set_pps(int pps) = 0;

protected:
    std::string _name;
};

//main DAC handling functions
void init_dacs();
DACList list_dacs();
DAC* dac_connect(std::string name);
