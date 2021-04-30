#include "Color.h"
#include "Ray.h"
#include "Vec3.h"
#include <iostream>
#include <fstream>

double hit_sphere(const Point3& center, double radius, const Ray& ray)
{
    Vec3 oc = ray.origin() - center;
    //ray = A +tB
    //C = center
    // t^2*(B.B) + 2t*B.(A-C) + (A-C).(A-C)-r^2 = 0
    auto a = ray.direction().length_squared();
    auto b = dot(oc, ray.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = b * b - a * c; // D = b^2 - 4ac = 4(b/2 ^2 - ac)
    
    if (discriminant < 0) return -1;
    else return (-b - sqrt(discriminant))/a;
}

Color ray_color(const Ray& ray) //DECIDES COL0R ON A RAYTRACE
{
    auto center = Point3(0.0, 0.0, -1.0);//CENTER OF SPHERE
    auto t = hit_sphere(center, 0.5, ray);
    if (t > 0.0)
    {
        Vec3 N = unit_vector(ray.at(t) - center);
        return 0.5 * Color(N.x() + 1, N.y() + 1, N.y() + 1);
    }
    Vec3 unit_direction = unit_vector(ray.dir);
    t = 0.5 * (unit_direction.y() + 1.0);
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

    //WRITING TO PPM FORMAT IMAGE
    ofstream image{ "render.ppm", ios::out | ios::trunc };
    image << "P3\n" << iWidth << ' ' << iHeight << "\n255\n";
    for (int j = iHeight - 1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < iWidth; ++i) 
        {
            //SCREEN COORDINATES = FULL RESOLUTION / INDEX OF PIXEL
            auto u = double(i) / (iWidth - 1.0);
            auto v = double(j) / (iHeight - 1.0);

            Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
            Color pixel_color = ray_color(ray);
            write_color(image, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}