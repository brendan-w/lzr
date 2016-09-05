
#pragma once

#include <liblzr.h>
#include "signals.h"


/*
 * A parameter of an Effect. Can accept values from any signal
 * of its own SignalType.
 */
class EffectParam
{
public:
    EffectParam(SignalType type) {
        //populate the signal choices
        sigs = signals_of_type(type);
    };

    ~EffectParam() {
        for(Signal* s : sigs) { delete s; }
    };

    Signal* signal() {
        return sigs[s];
    };

    //all of the possible signal types
    QList<Signal*> sigs;
    int s;
};


/*
 * Base effect class. Defines API for processing LZR frames
 * at a given time in the show or clip.
 */
class Effect
{
public:
    Effect(QString name, QMap<QString, EffectParam*> params) :
        name(name), params(params) {};

    virtual ~Effect() {
        for(EffectParam* p : params) { delete p; }
    };

    virtual void run(lzr::Frame& frame, Time& t) = 0;

    bool active;
    const QString name;
    const QMap<QString, EffectParam*> params;
};



/*
 * EFFECTS
 */

//The base frame/animation generator
class FrameEffect : public Effect
{
public:
    FrameEffect() : Effect("Frame", {
            {"Frame Number", new EffectParam(DOUBLE)},
        }) {};

    void run(lzr::Frame& frame, Time& t)
    {
        int n = params["Frame Number"]->signal()->double_value(t);
        n = qMin(frames.size(), qMax(0, n)); //clamp
        frame = frames[n];
    };

private:
    QList<lzr::Frame> frames;
};



class MoveEffect : public Effect
{
public:
    MoveEffect() : Effect("Move", {
            {"X", new EffectParam(DOUBLE)},
            {"Y", new EffectParam(DOUBLE)}
        }) {};

    void run(lzr::Frame& frame, Time& t)
    {
        lzr::translate(frame,
                       params["X"]->signal()->double_value(t),
                       params["Y"]->signal()->double_value(t));
    };
};
