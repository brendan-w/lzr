
#pragma once

#include <liblzr.h>
#include <QObject>
#include <QColor>
#include "utils.hpp"


/*
 * This is only for efficient matching of Signal models to their UI counterparts
 */
enum SignalType {
    CONSTANT=0,
    CURVE
};


/*
 * Base signal class. Only stores information neccessary for typecasting in the UI
 */
class Signal : public QObject
{
    Q_OBJECT
public:
    Signal(SignalType type) : type(type) {};
    virtual ~Signal() {};
    const SignalType type;
};


/*
 * Signal Catagory subclasses. Catagorized by output value type
 */

class DoubleSignal : public Signal
{
    Q_OBJECT
public:
    DoubleSignal(SignalType type, double min, double max) : Signal(type)
    {
        v_min = min;
        v_max = max;
    };

    virtual double value(Time& t) = 0;
    double min() { return v_min; };
    double max() { return v_max; };

signals:
    void valueChanged(double v);

public slots:
    void setMin(double min) { v_min = min; };
    void setMax(double max) { v_max = max; };

protected:
    double output_clamp(double v) { return clamp(v, v_min, v_max); };

private:
    double v_min;
    double v_max;
};


/*
 * Signal emitting DOUBLES
 */

class ConstantSignal : public DoubleSignal
{
    Q_OBJECT
public:
    ConstantSignal(double min, double max) : DoubleSignal(CONSTANT, min, max) {};
    double value(Time& t) { Q_UNUSED(t); return v; };

public slots:
    void setValue(double value)
    {
        v = output_clamp(value);
        emit valueChanged(v);
    };

private:
    double v = 0.0;
};




class CurvePoint
{
public:
    double x;
    double y;
    QPointF left;  //bezier handles/control points
    QPointF right;
    bool left_is_bezier; //flag for whether the left side is a cubic bezier curve
};


class CurveSignal : public DoubleSignal
{
    Q_OBJECT
public:
    CurveSignal(double min, double max) : DoubleSignal(CURVE, min, max) {};
    double value(Time& t) { Q_UNUSED(t); return 1.0; };

    CurvePoint* addPoint(QPointF pos);
    bool removePoint(CurvePoint* p);
    void movePoint(CurvePoint* p, QPointF pos);

private:
    double input_clamp(double v) { return clamp(v, 0, 1); };
    QList<CurvePoint*> points;
};
