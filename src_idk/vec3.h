#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"

struct vec3
{
    double X = 0.0, Y = 0.0, Z = 0.0;

    vec3 operator-() const
    {
        return {-X, -Y, -Z};
    }

    vec3& operator+=(const vec3& V)
    {
        X += V.X;
        Y += V.Y;
        Z += V.Z;
        return *this;
    }

    vec3& operator*=(double T)
    {
        X *= T;
        Y *= T;
        Z *= T;
        return *this;
    }

    vec3& operator/=(double T)
    {
        // Note: t being zero will be bad
        return *this *= 1/T;
    }

    double LengthSquared() const
    {
        return X*X + Y*Y + Z*Z;
    }

    double Length() const
    {
        return sqrt(LengthSquared());
    }

    bool NearZero() const
    {
        // Returns true if the vector is close to zero in all dimensions
        double s = 1e-8;
        return (fabs(X) < s) && (fabs(Y) < s) && (fabs(Z) < s);
    }
};

// Alias that will be useful for geometric clarity in the code
typedef vec3 point3;

vec3 RandomVec3()
{
    return { RandomDouble(), RandomDouble(), RandomDouble() };
}

vec3 RandomVec3(double Min, double Max)
{
    return { RandomDouble(Min,Max), RandomDouble(Min,Max), RandomDouble(Min,Max) };
}

// Vector utility functions
inline vec3 operator+(const vec3& U, const vec3& V)
{
    return { U.X + V.X, U.Y + V.Y, U.Z + V.Z };
}

inline vec3 operator-(const vec3& U, const vec3& V)
{
    return { U.X - V.X, U.Y - V.Y, U.Z - V.Z };
}

inline vec3 operator*(const vec3& U, const vec3& V)
{
    return { U.X * V.X, U.Y * V.Y, U.Z * V.Z };
}

inline vec3 operator*(double T, const vec3& V)
{
    return { T*V.X, T*V.Y, T*V.Z };
}

inline vec3 operator*(const vec3& V, double T)
{
    return T * V;
}

inline vec3 operator/(const vec3& V, double T)
{
    // Note: Again, T being zero will be bad
    return (1/T) * V;
}

inline double Dot(const vec3& U, const vec3& V)
{
    return U.X*V.X + U.Y*V.Y + U.Z*V.Z;
}

inline vec3 Cross(const vec3& U, const vec3& V)
{
    return { U.Y*V.Z - U.Z*V.Y,
             U.Z*V.X - U.X*V.Z,
             U.X*V.Y - U.Y*V.X };
}

inline vec3 UnitVector(const vec3& V)
{
    double LenSq = V.LengthSquared();
    if(LenSq > 0)
    {
        double InvLen = 1/sqrt(LenSq);
        return V * InvLen;
    }
    return V;
}

inline vec3 RandomUnitVector()
{
#if 0
    // This approach generates random vectors in the unit cube, normalizes it
    // and returns if the vector is in unit sphere
    while(true)
    {
        vec3 V = RandomVec3(-1, 1);
        double LenSq = V.LengthSquared();
        if(1e-160 < LenSq <= 1)
            return V / sqrt(LenSq);
    }
#else
    // Probably a better approach: Generate random spherical coordinates
    double theta = 2 * Pi * RandomDouble(); // Azimuthal angle
    double z  = 2 * RandomDouble() - 1;     // z ∈ [-1,1]
    double r = sqrt(1 - z*z);
    return {r*cos(theta), r*sin(theta), z};
#endif
}

inline vec3 RandomInUnitDisk()
{
#if 0
    while(true)
    {
        vec3 V = { RandomDouble(-1,1), RandomDouble(-1,1), 0 } 
        if(V.LengthSquared() < 1)
            return V;
    }
#else
    // Using polar coordinates
    double r = sqrt(RandomDouble()); // Sqrt for uniform distribution in disk (idk how this works tbh)
    double theta = 2 * Pi * RandomDouble();
    return { r*cos(theta), r*sin(theta), 0 };
#endif
}

inline vec3 RandomOnHemisphere(const vec3& Normal)
{
    vec3 OnUnitSphere = RandomUnitVector();
    if(Dot(OnUnitSphere, Normal) > 0.0) // In the same hemisphere as the normal
        return OnUnitSphere;
    else
        return -OnUnitSphere;
}

inline vec3 Reflect(const vec3& V, const vec3& N)
{
    // V and N point in opposite directions, hence the minus sigh to get the correct direction for the vector b
    // Which is (-ProjectionOf_V_On_N * n) (see figure 15)
    return V - 2*Dot(V,N)*N;
}

inline vec3 Refract(const vec3& R, const vec3& N, double EtaOverEtaPrime)
{
    double CosTheta = fmin(Dot(-R,N), 1.0);
    vec3 ROutPerp = EtaOverEtaPrime * (R + CosTheta*N);
    vec3 ROutParallel = -sqrt(fabs(1.0 - ROutPerp.LengthSquared())) * N;
    return ROutPerp + ROutParallel;
}

#endif