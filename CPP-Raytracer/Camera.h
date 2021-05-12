/*
* Camera.h : 
*/
#ifndef CAMERA_H
#define CAMERA_H

#include "RTMath.h"

class Camera
{
public :
    Camera(Point3 lookFrom, Point3 lookAt, Vec3 vUp, double vfov, double aspectRatio, double aperture, double focus_dist, double _time0 = 0.0, double _time1 = 0.0)
    {
        //SEE NOTES FOR EXPLANATION
        auto theta = deg_to_rad(vfov);
        auto h = tan(theta / 2.0);
        auto viewportHeight = 2.0 * h;
        auto viewportWidth = viewportHeight * aspectRatio;

        w = unit_vector(lookFrom - lookAt);
        u = unit_vector(cross(vUp, w));
        v = cross(w, u);

        origin = lookFrom;
        horizontal = focus_dist * viewportWidth * u;
        vertical = focus_dist * viewportHeight * v;
        lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
        lens_radius = aperture / 2.0;

        time0 = _time0;
        time1 = _time1;
    }

    Ray get_ray(double s, double t) // RETURNS A RAY THROUGH PIXEL COORDS(S,T). SEE NOTES FOR FULL EXPLANATION FOR RD AND OFFSET
    {
        Vec3 rd = lens_radius * random_vec_disk();
        Vec3 offset = u * rd.x() + v * rd.y();
        return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset, random(time0, time1));
    }

private :
    Point3 origin; // ORIGIN OF CAMERA AND THUS, THE WORLD
    Point3 lowerLeftCorner; // LOWERLEFT POINT COORDS OF LOWER LEFT CORNER OF VIEWPORT
    Vec3 horizontal;
    Vec3 vertical;

    Vec3 u, v, w;
    double lens_radius; // APERTURE

    double time0, time1;
};
#endif