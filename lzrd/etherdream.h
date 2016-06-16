
#pragma once

#include "dac.h"
#include "libetherdream/etherdream.h"


class EtherDream : public DAC
{
public:
    static DACList list_dacs();

    EtherDream(std::string name);
    ~EtherDream();

    int write(Frame frame);
    int stop();
    int set_pps(int pps);

private:
    const std::string prefix;
    struct etherdream* dac;
    int pps;
};
