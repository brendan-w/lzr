
#pragma once

#include "dac.h"
#include "libetherdream/etherdream.h"


class EtherDream : public DAC
{
public:
    static DACList list_dacs();

    EtherDream(std::string name);
    ~EtherDream();

    int send(Frame frame);
    int stop();
    int set_pps(int pps);

private:
    struct etherdream* dac;
    std::vector<struct etherdream_point> buffer;
    int pps;
    int success;
};
