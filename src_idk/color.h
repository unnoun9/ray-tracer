#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

typedef vec3 color;

inline double LinearToGamma(double LinearComponent)
{
    if(LinearComponent > 0)
        return std::sqrt(LinearComponent);
    return 0;
}

void WriteColor(const color& PixelColor)
{
    double R = PixelColor.X;
    double G = PixelColor.Y;
    double B = PixelColor.Z;

    // Apply a linear to gamma transfor for gamma 2
    R = LinearToGamma(R);
    G = LinearToGamma(G);
    B = LinearToGamma(B);

    // Translate the [0,1] component values to the byte range [0,255]
    static interval intensity = {0.000, 0.999};
    int RByte = (int)(256 * intensity.Clamp(R));
    int GByte = (int)(256 * intensity.Clamp(G));
    int BByte = (int)(256 * intensity.Clamp(B));

    // Write out the pixel color components
    fprintf(stdout, "%i %i %i\n", RByte, GByte, BByte);
}
#endif