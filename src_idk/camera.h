#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include "vec3.h"
#include "sphere.h"

struct camera
{
    double AspectRatio = 1.0;   // Retio of the image width over height
    int ImageWidth = 100;       // Rendered image width in pixel count
    int SamplesPerPixel = 10;   // Count of random samples for each pixel
    int MaxDepth = 10;          // Maximum number of ray bounces into the scene

    double VFov = 90;                   // Vertical view angle (field of view)
    point3 LookFrom = {0, 0, 0};        // Point camera is looking from
    point3 LookAt = {0, 0, 0};          // Point camera is looking at
    vec3 VUp = {0, 1, 0};               // Camera-relative "up" direction

    double DefocusAngle = 0;    // Variation angle of rays through each pixel
    double FocusDist = 10;      // Distance from camera LookFrom point to plane of perfect focus

    int ImageHeight;            // Rendered image height
    double PixelSamplesScale;   // Color scale factor for a sum of pixel samples
    point3 Center;              // Camera center
    point3 Pixel00Loc;          // Location of pixel (0, 0)
    vec3 PixelDeltaU;           // Offset to pixel to the right
    vec3 PixelDeltaV;           // Offset to pixel below
    vec3 U, V, W;               // Camera frame basis vectors
    vec3 DefocusDiskU;          // Defocus disk horizontal radius
    vec3 DefocusDiskV;          // Defocus disk vertical radius
};

// Create a camera freely via filling camera struct manually, or with some rules and restrictions via this function
camera InitializeCamera(int AspectRatio, int ImageWidth, int SamplesPerPixel, int MaxDepth,
                      double VFov, point3* LookFrom, point3* LookAt, vec3* VUp,
                      double DefocusAngle, double FocusDist)
{
    camera Camera;

    Camera.AspectRatio = AspectRatio;
    Camera.ImageWidth = ImageWidth;
    Camera.SamplesPerPixel = SamplesPerPixel;
    Camera.MaxDepth = MaxDepth;
    Camera.VFov = VFov;
    Camera.LookFrom = *LookFrom;
    Camera.LookAt = *LookAt;
    Camera.VUp = *VUp;
    Camera.DefocusAngle = DefocusAngle;
    Camera.FocusDist = FocusDist;

    // Calculate the image height, and ensure that it's at least 1
    Camera.ImageHeight = (int)(ImageWidth / AspectRatio);
    Camera.ImageHeight = (Camera.ImageHeight < 1) ? 1 : Camera.ImageHeight;

    Camera.PixelSamplesScale = 1.0 / SamplesPerPixel;

    Camera.Center = *LookFrom;
    
    // Determine viewport dimensions
    double theta = DegreesToRadians(VFov);
    double h = tan(theta/2);
    double ViewportHeight = 2 * h * FocusDist;
    double ViewportWidth = ViewportHeight * (double)(ImageWidth/Camera.ImageHeight);

    // Calculate the U, V, W unit basis vectors for the camera coordinate frame
    Camera.W = UnitVector(*LookFrom - *LookAt);
    Camera.U = UnitVector(Cross(*VUp, Camera.W));
    Camera.V = Cross(Camera.W, Camera.U);

    // Calculate the vectors accross the horizontal and down the vertical viewport edges
    vec3 ViewportU = ViewportWidth * Camera.U;      // Vector across viewport horizontal edge
    vec3 ViewportV = ViewportHeight * -Camera.V;    // Vector down viewport vertical edge

    // Calculate te horizontal and vertical elta vectors from pixel to pixel
    Camera.PixelDeltaU = ViewportU / ImageWidth;
    Camera.PixelDeltaV = ViewportV / Camera.ImageHeight;

    // Calculate the location of the upper left pixel
    vec3 ViewportUpperLeft = Camera.Center - (FocusDist * Camera.W) - ViewportU/2 - ViewportV/2;
    Camera.Pixel00Loc = ViewportUpperLeft + 0.5 * (Camera.PixelDeltaU + Camera.PixelDeltaV);

    // Calculate the camera defocus disk basis vectors
    double DefocusRadius = FocusDist * tan(DegreesToRadians(DefocusAngle / 2));
    Camera.DefocusDiskU = Camera.U * DefocusRadius;
    Camera.DefocusDiskV = Camera.V * DefocusRadius;

    return Camera;
}

void RenderCamera(camera *Camera, sphere Spheres[], int NumSpheres)
{

}

#endif