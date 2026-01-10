#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct ray
{
    point3 Orig;
    vec3 Dir;

    point3 At(double T) const
    {
        return Orig + T*Dir;
    }
};

#endif