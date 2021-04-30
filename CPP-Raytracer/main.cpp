#include "Color.h"
#include "Ray.h"
#include "Vec3.h"
#include <iostream>
#include <fstream>

bool hit_sphere(const Point3& center, double radius, const Ray& ray)
{
    Vec3 oc = ray.origin() - center;
    //ray = A +tB
    //C = center
    // t^2*(B.B) + 2t*B.(A-C) + (A-C).(A-C)-r^2 = 0
    auto a = dot(ray.direction(), ray.direction());
    auto b = 2.0 * dot(oc, ray.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

Color ray_color(const Ray& ray)
{
    if (hit_sphere(Point3(0, 0, -1), 0.5, ray))
    {
        return Color(1.0, 0.0, 0.0);
    }
    Vec3 unit_direction = unit_vector(ray.dir);
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

using namespace std;
int main() 
{
    //IMAGE DIMENSIONS
    const auto aspectRatio = 16.0 / 9.0;
    const int iWidth = 1920;
    const int iHeight = static_cast<int>(iWidth/aspectRatio);

    //CAMERA
    auto viewportHeight = 2.0;
    auto viewportWidth = viewportHeight * aspectRatio;
    auto focalLength = 1.0;

    auto origin = Point3(0, 0, 0);
    auto horizontal = Vec3(viewportWidth, 0, 0);
    auto vertical = Vec3(0, viewportHeight, 0);
    auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);

    ofstream image{ "render.ppm", ios::out | ios::trunc };
    image << "P3\n" << iWidth << ' ' << iHeight << "\n255\n";
    for (int j = iHeight - 1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < iWidth; ++i) 
        {
            auto u = double(i) / (iWidth - 1.0);
            auto v = double(j) / (iHeight - 1.0);
            Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
            Color pixel_color = ray_color(r);
            write_color(image, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}