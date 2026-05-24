#ifndef HIT_H
#define HIT_H

#include "vec3.h"
#include "ray.h"
#include "sphere.h"

struct material;

struct hit_record
{
    point3 P;           // The location of hit
    vec3 Normal;        // Normal vector that points away from the incident ray (design decision)
    material *Material; // The material of the object that was hit
    double T;           // How far or close the hit is
    bool FrontFace;     // True if ray is outside, false if ray is inside

    void SetFaceNormal(ray *R, vec3 *OutwardNormal)
    {
        // Sets the hit record normal vector
        // Note: The parameeter `OutwardNormal` is assumed to have unit length
        // And this vector is the normal that points outwards
        
        // Negative dot product means vectors are opposite in direction
        FrontFace = Dot(R->Dir, *OutwardNormal) < 0;
        Normal = FrontFace ? *OutwardNormal : -(*OutwardNormal);
    }
};

// TODO: Make this take a list of tagged unions maybe?
bool HitOnSpheres(sphere *Spheres, int NumSpheres, ray *R, interval RayT, hit_record *Record)
{
    hit_record TempRecord;
    bool HitAnything = false;
    double ClosestSoFar = RayT.Max;

    for(int i = 0; i < NumSpheres; ++i)
    {
        if(Spheres[i].Hit(R, {RayT.Min, ClosestSoFar}, &TempRecord))
        {
            HitAnything = true;
            ClosestSoFar = TempRecord.T;
            (*Record) = TempRecord;
        }
    }

    return HitAnything;
}

#endif
