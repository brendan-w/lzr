
#pragma once

#include "dac.h"
#include "libetherdream/etherdream.h"

namespace lzr {


class EtherDream : public DAC
{
public:
    static DACList list_dacs();

    EtherDream(std::string name);
    ~EtherDream();

    int send(Frame frame);
    int stop();
    void pps(int pps);
    int pps();

private:
    struct etherdream* dac;
    std::vector<struct etherdream_point> buffer;
    int _pps;
};

} // namespace lzr
