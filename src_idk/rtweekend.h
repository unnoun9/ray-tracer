#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <limits>

// Constants
const double Infinity = std::numeric_limits<double>::infinity();
const double Pi = 3.1415926535897932385;

// Utility functions
inline double DegreesToRadians(double Degrees)
{
    return Degrees * Pi / 180;
}

// Returns a random real in [0, 1)
inline double RandomDouble()
{
    return rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double Min, double Max)
{
    return Min + (Max-Min)*RandomDouble();
}

#endif