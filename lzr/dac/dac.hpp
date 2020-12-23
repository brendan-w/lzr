#pragma once

#include "lzr/core/core.hpp"

namespace lzr {


/******************************************************************************/
/*  LZR DAC Handling                                                          */
/******************************************************************************/

//fwrd decl
class DAC_Internals;

//the standard interface for DACS
//do NOT instantiate this class yourself
//use the dac_connect() factory function below
class DAC
{
public:
    DAC(const std::string& name);
    virtual ~DAC();

    //standard DAC interface
    std::string name();
    bool connected();

    //DAC specific functions
    virtual int send(Frame frame) = 0;
    virtual int stop() = 0;
    virtual void pps(int pps);
    virtual int pps();

protected:
    DAC_Internals* internal;
};


//initialize individual DAC backends / prepare for hardware probing
void init_dacs();

//get list of connected DACs
typedef std::set<std::string> DACNames;
DACNames list_dacs();

//connect to a DAC
DAC* dac_connect(const std::string& name);

} // namespace lzr
