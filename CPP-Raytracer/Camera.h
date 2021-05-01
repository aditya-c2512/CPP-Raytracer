#ifndef CAMERA_H
#define CAMERA_H

#include "RTMath.h"

class Camera
{
public :
    Camera()
    {
        const auto aspectRatio = 16.0 / 9.0;
        auto viewportHeight = 2.0;
        auto viewportWidth = viewportHeight * aspectRatio;
        auto focalLength = 1.0;

        origin = Point3(0, 0, 0);
        horizontal = Vec3(viewportWidth, 0, 0);
        vertical = Vec3(0, viewportHeight, 0);
        lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);
    }

    Ray get_ray(double u, double v)
    {
        return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
    }

private :
    Point3 origin;
    Point3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
};
#endif