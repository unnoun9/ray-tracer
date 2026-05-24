#include "rtweekend.h"
#include "material.h"
#include "sphere.h"

int main()
{
    // srand(time(0));

    int eleven = 11; // Avoid magic number >.<
    // The ground, the three main ones, and all the randomly generated spheres
    int MaxSpheres = (eleven - (-eleven)) * (eleven - (-eleven)) + 4;
    sphere Spheres[MaxSpheres];

    material GroundMaterial = {LAMBERTIAN, {0.5, 0.5, 0.5}};
    int Index = 0;
    Spheres[Index++] = { {0,-1000,0}, 1000, GroundMaterial };

    for(int A = -eleven; A < eleven; ++A)
    {
        for(int B = -eleven; B < eleven; ++B)
        {
            double ChooseMatProbability = RandomDouble();
            point3 Center = {A + 0.9*RandomDouble(), 0.2, B + 0.9*RandomDouble()};
            point3 Reference = {4, 0.2, 0};

            if((Center - Reference).Length() > 0.9)
            {
                // TODO: Break if Index > MaxSphere - 3?

                material SphereMaterial = {};

                if (ChooseMatProbability < 0.8)
                {
                    // Diffuse
                    color Albedo = RandomVec3() * RandomVec3();
                    SphereMaterial.Type = LAMBERTIAN;
                    SphereMaterial.Albedo = Albedo;
                    Spheres[Index] = { Center, 0.2, SphereMaterial };
                }
                else if (ChooseMatProbability < 0.95)
                {
                    // Metal
                    color Albedo = RandomVec3(0.5, 1);
                    double Fuzz = RandomDouble(0, 0.5);
                    SphereMaterial.Type = METAL;
                    SphereMaterial.Albedo = Albedo;
                    SphereMaterial.Fuzz = Fuzz;
                    Spheres[Index] = { Center, 0.2, SphereMaterial };
                }
                else
                {
                    // Glass
                    SphereMaterial.Type = DIELECTRIC;
                    SphereMaterial.RefractionIndex = 1.5;
                    Spheres[Index] = { Center, 0.2, SphereMaterial };
                }
                ++Index;
            }
        }
    }

    material Material = {};
    Material.Type = DIELECTRIC;
    Material.RefractionIndex = 1.5;
    Spheres[Index++] = { {0,1,0}, 1, Material };
    
    Material.Type = LAMBERTIAN;
    Material.Albedo = {0.4, 0.2, 0.1};
    Spheres[Index++] = { {-4,1,0}, 1, Material };
    
    Material.Type = METAL;
    Material.Albedo = {0.7,0.6,0.5};
    Material.Fuzz = 0;
    Spheres[Index++] = { {4,1,0}, 1, Material };

    fprintf(stderr, "Spheres created: %i\n", Index);

    // Camera...
}