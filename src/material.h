#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
#include "color.h"
#include "hit.h"

enum material_type
{
    LAMBERTIAN = 0, METAL, DIELECTRIC, NUM_MAT_TYPES
};

double Reflectance(double cosine, double RefractionIndex)
{
    // Use Schlick's approximation for reflectance
    double R0 = (1 - RefractionIndex) / (1 + RefractionIndex);
    R0 = R0 * R0;
    return R0 + (1-R0)*pow((1-cosine), 5);
}

struct material
{
    material_type Type;         // Tag
    color Albedo;               // Fractional reflectance in all the r,g,b channels
    double Fuzz;                // (For METALs only) Amount of fuzziness in reflections
    double RefractionIndex;     // (For DIELECTRICs only) Refractive index in vaccum or air
                                // Or the ratio of the material's refractive index over the refractive index of the enclosing media

    bool Scatter(ray *RIn, hit_record *Record, color *Attenuation, ray *Scattered)
    {
        switch(Type)
        {
            case LAMBERTIAN:
            {
                // We are here always scattering and always scaling by Albedo
                // Other approaches inlude:
                // 1) Scatter with probability of albedo and don't scale
                // 2) Scatter with probability of P and scale energy by Albedo/p (note that this approach is the combination of 0 and 1)
        
                vec3 ScatterDirection = Record->Normal + RandomUnitVector();

                // Catch degenerate scatter direction
                if (ScatterDirection.NearZero())
                    ScatterDirection = Record->Normal;

                Scattered->Orig = Record->P;
                Scattered->Dir = ScatterDirection;
                (*Attenuation) = Albedo;
                return true;
            } break;

            case METAL:
            {
                vec3 Reflected = Reflect(RIn->Dir, Record->Normal);
                Reflected = UnitVector(Reflected) + (Fuzz * RandomUnitVector());
                Scattered->Orig = Record->P;
                Scattered->Dir = Reflected;
                (*Attenuation) = Albedo;
                // Because of fuzziness, a ray on a big object or a ray with a direction very close to the surface
                // May reflect into the object, so discard/absorb such rays
                return Dot(Scattered->Dir, Record->Normal) > 0;
            } break;

            case DIELECTRIC:
            {
                (*Attenuation) = { 1.0, 1.0, 1.0 };
                // If front face then it should be eta/eta_prime, otherwise its eta_prime/eta
                // This is for the refraction function
                double RI = Record->FrontFace ? (1.0 / RefractionIndex) : RefractionIndex;

                vec3 UnitDirection = UnitVector(RIn->Dir);

                double CosTheta = fmin(Dot(-UnitDirection, Record->Normal), 1.0);
                double SinTheta = sqrt(1.0 - CosTheta * CosTheta);

                bool CannotRefract = RI * SinTheta > 1.0; // Total internal reflection
                vec3 Direction;
                if (CannotRefract || Reflectance(CosTheta, RI) > RandomDouble())
                    Direction = Reflect(UnitDirection, Record->Normal);
                else
                    Direction = Refract(UnitDirection, Record->Normal, RI);

                (*Scattered) = { Record->P, Direction };
                return true;
            } break;
        }
    }
};

#endif