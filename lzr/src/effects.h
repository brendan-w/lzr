
#pragma once

#include <liblzr.h>
#include <QObject>
#include "signals.h"


/*
 * This base class is mostly for ease of iterating
 * over possible signal sources in the UI
 */

class Param
{
public:
    virtual ~Param()
    {
        for(Signal* s : sigs)
            delete s;
    };

    Signal* current;
    QMap<SignalType, Signal*> sigs;
};


class DoubleParam : public Param
{
public:
    DoubleParam(double min, double max)
    {
        //all of the possible signals
        sigs[CONSTANT] = new ConstantSignal(min, max);
        sigs[CURVE]    = new CurveSignal(min, max);
        current = sigs[CONSTANT];
    };

    double value(Time& t)
    {
        return ((DoubleSignal*)current)->value(t);
    };
};



/*
 * Base effect class. Defines API for processing LZR frames
 * at a given time in the show or clip.
 */
class Effect
{
public:
    Effect(QString name) : name(name) {};
    virtual ~Effect()
    {
        for(Param* p : params)
            delete p;
    };

    virtual void run(lzr::Frame& frame, Time& t) = 0;

    bool active;
    const QString name;
    QMap<QString, Param*> params;
};

//helpers for accessing values of effect parameters
#define DOUBLE(p, t) (((DoubleParam*) params[p])->value(t))


/*
 * EFFECTS
 */


class FrameEffect : public Effect
{
public:
    FrameEffect() : Effect("Frame")
    {
        params["Frame Number"] = new DoubleParam(0.0, 0.0);
    };

    void run(lzr::Frame& frame, Time& t)
    {
        int n = DOUBLE("Frame Number", t);
        frame = frames[n]; //TODO, make safe
    };

private:
    QList<lzr::Frame> frames;
};





class MoveEffect : public Effect
{
public:
    MoveEffect() : Effect("Move")
    {
        params["X"] = new DoubleParam(-1.0, 1.0);
        params["Y"] = new DoubleParam(-1.0, 1.0);
    };

    void run(lzr::Frame& frame, Time& t)
    {
        lzr::translate(frame,
                       DOUBLE("X", t),
                       DOUBLE("Y", t));
    };
};
