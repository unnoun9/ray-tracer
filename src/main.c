#include "../../lib/lib/base.h"
#include "../../lib/lib/mathematics.h"
#include <time.h>

#define ELEVEN 11
#define NUM_SMOL_SPHERES ((ELEVEN - (-ELEVEN)) * (ELEVEN- (-ELEVEN)))

typedef enum
{
    MAT_LAMBERTIAN, MAT_METAL, MAT_DIELECTRIC, NUM_MATS
} material_type;

typedef struct
{
    vec3 center;
    f32 radius;
    material_type mat_type;
    union
    {
        // lamertian
        // fractional reflectance in all the r, g, and b channels
        vec3 lambertian_albedo;

        // metal
        struct
        {
            vec3 metal_albedo;
            f32 fuzz;
        };

        // dielectric
        // refractive index in vaccum or air, or the ratio of the material's refractive index over the refractive index of the enclosing media
        f32 refraction_index; 
    };
} sphere;

typedef struct
{
    sphere *items;
    u32 size;
    u32 capacity;
} spheres_da;

typedef struct
{
    f32 aspect_ratio;           // ratio of the image width over height
    u32 image_width;            // rendered image width in pixel count
    u32 image_height;           // rendered image height
    u32 samples_per_pixel;      // count of random samples for each pixel
    u32 max_depth;              // maximum number of ray bounces into the scene
    f32 pixel_samples_scale;    // color scale factor for a sum of pixel samples
    vec3 pixel00_loc;           // location of pixel 0, 0
    vec3 pixel_delta_u;         // offset to pixel to the right
    vec3 pixel_delta_v;         // offset to pixel below

    f32 vfov;                   // vertical view angle (field of view)
    vec3 lookfrom;              // point camera is looking from
    vec3 lookat;                // point camera is looking at
    vec3 vup;                   // camera-relative "up" direction
    vec3 center;                // camera center
    vec3 u, v, w;               // camera frame basis vectors
    
    f32 defocus_angle;          // variation angle of rays through each pixel
    f32 focus_dist;             // distance from camera lookfrom point to plan of perfect focus
    vec3 defocus_disk_u;        // defocus disk horizontal radius
    vec3 defocus_disk_v;        // defocus disk vertical radius
} camera;

f64 delta_secs(struct timespec begin, struct timespec end)
{
    f64 a = (f64)begin.tv_sec + begin.tv_nsec*1e-9;
    f64 b = (f64)end.tv_sec + end.tv_nsec*1e-9;
    return b - a;
}

int main()
{
    spheres_da spheres = {0};
    da_reserve(&spheres, sizeof(*spheres.items) * (NUM_SMOL_SPHERES + 4));

    // ground
    sphere sphere_to_add;
    sphere_to_add.center = (vec3){0, -1000, 0};
    sphere_to_add.radius = 1000;
    sphere_to_add.mat_type = MAT_LAMBERTIAN;
    sphere_to_add.lambertian_albedo = (vec3){0.5f, 0.5f, 0.5f};
    da_append(&spheres, sphere_to_add);
    
    // smol spheres
    for (i32 a = -ELEVEN; a < ELEVEN; a++)
    {
        for (i32 b = -ELEVEN; b < ELEVEN; b++)
        {
            vec3 center = {a + 0.9f*randf(), 0.2f, b + 0.9f*randf()};
            f32 dist_from_center = vec3_dist(center, (vec3){4, 0.2f, 0});

            if (dist_from_center > 0.9f)
            {
                f32 choose_mat = randf();

                if (choose_mat < 0.8f)
                {
                    // diffuse
                    sphere_to_add.center = center;
                    sphere_to_add.radius = 0.2f;
                    sphere_to_add.mat_type = MAT_LAMBERTIAN;
                    sphere_to_add.lambertian_albedo = vec3_mul(rand_vec3(), rand_vec3());
                }
                else if (choose_mat < 0.95f)
                {
                    // metal
                    sphere_to_add.center = center;
                    sphere_to_add.radius = 0.2f;
                    sphere_to_add.mat_type = MAT_METAL;
                    sphere_to_add.lambertian_albedo = rand_vec3_in(0.5f, 1);
                    sphere_to_add.fuzz = randf_in(0, 0.5f);
                }
                else
                {
                    // glass
                    sphere_to_add.center = center;
                    sphere_to_add.radius = 0.2f;
                    sphere_to_add.mat_type = MAT_DIELECTRIC;
                    sphere_to_add.refraction_index = 1.5;
                }

                da_append(&spheres, sphere_to_add);
            }
        }
    }

    // big spheres in the center
    sphere_to_add.center = (vec3){0,1,0};
    sphere_to_add.radius = 1;
    sphere_to_add.mat_type = MAT_DIELECTRIC;
    sphere_to_add.refraction_index = 1.5f;
    da_append(&spheres, sphere_to_add);
    sphere_to_add.center = (vec3){-4,1,0};
    sphere_to_add.radius = 1;
    sphere_to_add.mat_type = MAT_LAMBERTIAN;
    sphere_to_add.lambertian_albedo = (vec3){0.4f, 0.2f, 0.1f};
    da_append(&spheres, sphere_to_add);
    sphere_to_add.center = (vec3){4,1,0};
    sphere_to_add.radius = 1;
    sphere_to_add.mat_type = MAT_METAL;
    sphere_to_add.metal_albedo = (vec3){0.7f, 0.6f, 0.5f};
    sphere_to_add.fuzz = 0;
    da_append(&spheres, sphere_to_add);

    struct timespec begin = {0};
    i32 ret = clock_gettime(CLOCK_MONOTONIC, &begin);
    _assert(ret == 0, "clock_gettime(CLOCK_MONOTONIC, &begin) failed");

    // render the scene
    camera cam;
    cam.aspect_ratio = 1.0f;
    cam.image_width = 100;
    cam.samples_per_pixel = 10;
    cam.max_depth = 10;
    // ...initialize other related fields


    cam.vfov = 90;
    cam.lookfrom = (vec3){0,0,0};
    cam.lookat = (vec3){0,0,-1};
    cam.vup = (vec3){0,1,0};
    // ...initialize other related fields


    cam.defocus_angle = 0;
    cam.focus_dist = 10;
    // ...initialize other related fields


    struct timespec end = {0};
    ret = clock_gettime(CLOCK_MONOTONIC, &end);
    _assert(ret == 0, "clock_gettime(CLOCK_MONOTONIC, &end) failed");
    _put("Time for rendering took: %.3lfs", delta_secs(begin, end));

    return 0;
}