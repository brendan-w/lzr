
#pragma once

#include <liblzr.h>
#include <QAbstractListModel>
#include <QColor>


typedef struct {
    double actual;     // seconds since show start
    double normalized; // 0-1 normalized for the clip in question
} Time;


enum SignalType {
    DOUBLE,
    COLOR
};



class Signal
{
public:
    Signal(SignalType type) : type(type) {};
    virtual ~Signal() {};

    //accessor stubs. reimplement appropriately in subclass
    virtual double double_value(Time& t) { Q_UNUSED(t); return 0; };
    virtual QColor color_value(Time& t)  { Q_UNUSED(t); return QColor(); };

    const SignalType type;
};


class ConstantSignal : public Signal
{
public:
    ConstantSignal(SignalType type) : Signal(type) {};
    double double_value(Time& t) { return 1; };
};





class EffectParam
{
public:
    EffectParam(SignalType type) {
        switch(type)
        {
        case DOUBLE:
            sigs = { new ConstantSignal(type) };
        }
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





class Effect
{
public:
    Effect(QString name, QMap<QString, EffectParam*> params) :
        name(name), params(params) {};

    virtual ~Effect() {
        for(EffectParam* p : params) { delete p; }
    };

    virtual void run(lzr::Frame& frame, Time& t) = 0;

    const QString name;
    const QMap<QString, EffectParam*> params;
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




class Clip : public QAbstractListModel
{
    Q_OBJECT

public:
    Clip();

private:
    QList<Effect*> effects;
};

