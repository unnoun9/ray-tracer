#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

struct material
{
    virtual ~material() = default;

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
    {
        return false;
    }
};

struct lambertian : public material
{
    lambertian(const color& albedo)
        : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        // we are here always scattering and always scaling by albedo
        // other approaches include:
        // 1) scatter with probability of albedo and dont't scale
        // 2) scatter with probability p and scale energy by albedo/p (not that this approach is the combination of 0 and 1)

        auto scatter_direction = rec.normal + random_unit_vector();

        // catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;   // fractional reflectance in all the r, g, and b channels
};

struct metal : public material
{
    metal(const color& albedo, double fuzz)
        : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0; // because of fuzziness, a ray on a big object or a ray with a direction very close to the surface, may reflect into the object, so discard/absorb such rays
    }

private:
    color albedo;
    double fuzz;
};

struct dielectric : public material
{
    dielectric(double refraction_index)
        : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;   // if front face then it should be eta/eta_prime, otherwise it's eta_prime/eta; this is for the refraction function

        vec3 unit_direction = unit_vector(r_in.direction());

        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0; // total internal reflection
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri);

        scattered = ray(rec.p, direction);
        return true;
    }

private:
    double refraction_index; // refractive index in vaccum or air, or the ratio of the material's refractive index over the refractive index of the enclosing media

    static double reflectance(double cosine, double refraction_index)
    {
        // use schlick's approximation for reflectance
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1-r0)*std::pow((1-cosine), 5);
    }
};

#endif