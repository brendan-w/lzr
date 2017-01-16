
#include <engine.hpp>

namespace lzr {


json Constant::serialize()
{
    json j;
    j["v"] = v;
    return j;
}

void Constant::unserialize(json j)
{
    v = j["v"];
}



} // namespace lzr