
#pragma once

#include <liblzr.h>
#include <QAbstractListModel>
#include <QColor>
#include "signals.h"




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

