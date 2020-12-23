
#pragma once

#include "vendored/libetherdream/etherdream.h"

#include "dac_utils.hpp"

namespace lzr {


class EtherDream : public DAC
{
public:
    static DACNames list_dacs();

    EtherDream(std::string name);
    ~EtherDream();

    int send(Frame frame);
    int stop();

private:
    struct etherdream* dac;
    std::vector<struct etherdream_point> buffer;
};

} // namespace lzr
