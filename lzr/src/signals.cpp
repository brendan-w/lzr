

#include "signals.h"


QList<Signal*> signals_of_type(SignalType type)
{
    switch(type)
    {
    case DOUBLE:
        return {
            new ConstantSignal(),
            new CurveSignal()
        };
    default:
        return {};
    }
}
