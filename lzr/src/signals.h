
#pragma once

#include <liblzr.h>
#include <QAbstractListModel>
#include <QColor>
#include "utils.h"



enum SignalType {
    DOUBLE,
    COLOR
};


/*
 * Base signal class. Defines basic value generating functions with dummy definitions.
 * Subclasses need only implement the accessors for their SignalType.
 */
class Signal
{
public:
Signal(QString name, SignalType type) : name(name), type(type) {};
    virtual ~Signal() {};

    //accessor stubs, one for every SignalType
    //reimplement appropriately in subclass
    virtual double double_value(Time& t) { Q_UNUSED(t); return 0; };
    virtual QColor color_value(Time& t)  { Q_UNUSED(t); return QColor(); };

    const QString name;
    const SignalType type;
};

//factory for lists of applicable signals
QList<Signal*> signals_of_type(SignalType type);



/*
 * Signal emitting DOUBLES
 */

class ConstantSignal : public Signal
{
public:
    ConstantSignal() : Signal("Constant", DOUBLE) { value = 0.0; };
    double double_value(Time& t) { Q_UNUSED(t); return value; };
    double value;
};

class CurveSignal : public Signal
{
public:
    CurveSignal() : Signal("Curve", DOUBLE) {};
    double double_value(Time& t) { Q_UNUSED(t); return 1.0; };
};
