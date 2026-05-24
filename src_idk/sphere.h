#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "material.h"

struct sphere
{
    point3 Center;
    double Radius;
    material Material;

    // Return false if no solutions, or roots outside the [tmin,tmax] range
    // Fill the hit_record Record and return true if one of the roots is found (nearest in this case)
    bool Hit(ray *R, interval RayT, hit_record *Record)
    {
        vec3 OC = Center - R->Orig;

        // Simplified the quadratic formula calculations using the substition
        // b = -2h and the Dot(v, v) = v.LengthSquared();
        double A = R->Dir.LengthSquared();
        double H = Dot(R->Dir, OC);
        double C = OC.LengthSquared() - Radius*Radius;

        double Discriminant = H*H - A*C;
        if (Discriminant < 0)
            return false;
        
        double SqrtDisc = sqrt(Discriminant);

        // Find the nearest root that lies in the acceptable range
        double Root = (H - SqrtDisc) / A;
        if(!RayT.Surrounds(Root))
        {
            Root = (H + SqrtDisc) / A;
            if(!RayT.Surrounds(Root))
                return false;
        }

        Record->T = Root;
        Record->P = R->At(Record->T);
        vec3 OutwardsNormal = (Record->P - Center) / Radius;
        Record->SetFaceNormal(R, &OutwardsNormal);
        Record->Material = &Material;

        return true;
    }
};

#endif