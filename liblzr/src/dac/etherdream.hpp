
#pragma once

#include "dac.hpp"
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

private:
    struct etherdream* dac;
    std::vector<struct etherdream_point> buffer;
};

} // namespace lzr
