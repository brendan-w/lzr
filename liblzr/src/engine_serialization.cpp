
#include <engine.hpp>

namespace lzr {

/*
 * Helper to save and load values based on whether they're
 * constant, or have been mapped to inputs. Assumes that
 * a json object "j", and "input_map" are present in the scope.
 */
#define SAVE(variable) {                                  \
    if(input_map.find(&(variable)) != input_map.end())    \
        j[#variable] = input_map[&(variable)];            \
    else                                                  \
        j[#variable] = (variable);                        \
}

#define LOAD(variable) {                        \
    if(j[#variable].is_string())                \
        input_map[&variable] = j[#variable];    \
    else                                        \
        (variable) = j[#variable];              \
}




json Constant::serialize()
{
    json j;
    SAVE(v);
    return j;
}

void Constant::unserialize(json j)
{
    LOAD(v);
}



json Linear::serialize()
{
    json j;
    SAVE(x);
    SAVE(from_a);
    SAVE(from_b);
    SAVE(to_a);
    SAVE(to_b);
    return j;
}

void Linear::unserialize(json j)
{
    LOAD(x);
    LOAD(from_a);
    LOAD(from_b);
    LOAD(to_a);
    LOAD(to_b);
}



} // namespace lzr