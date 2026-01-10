#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

struct material;

struct hit_record
{
    point3 p;                   // the location of hit
    vec3 normal;                // normal vector that points away from the incident ray (design decision)
    shared_ptr<material> mat;   // the material of the object hit
    double t;                   // how far or close is the hit
    bool front_face;            // true if ray is outside, false if ray is inside    

    void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        // sets the hit record normal vector
        // note: the parameter `outward_normal` is assumed to have unit length and this vector is the normal that points outwards

        front_face = dot(r.direction(), outward_normal) < 0; // negative dot product means vectors are opposite in direction
        normal = front_face ? outward_normal : -outward_normal;
    }
};

struct hittable
{
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif