#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"

struct interval
{
    double Min = +Infinity, Max = -Infinity;
    static const interval Empty, Universe;

    double Size() const
    {
        return Max - Min;
    }

    bool Contains(double X) const
    {
        return Min <= X && X <= Max; // X in [Min,Max]
    }
    
    bool Surrounds(double X) const
    {
        return Min < X && X < Max;   // X in (Min,Max)
    }

    double Clamp(double X) const
    {
        if(X < Min) return Min;
        if(X > Max) return Max;
        return X;
    }
};

const interval interval::Empty    = {+Infinity, -Infinity};
const interval interval::Universe = {-Infinity, +Infinity};

#endif