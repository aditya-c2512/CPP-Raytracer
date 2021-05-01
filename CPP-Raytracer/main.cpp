#include "RTMath.h"

#include "Color.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Camera.h"
#include <iostream>
#include <fstream>

Color ray_color(const Ray& ray, const Hittable& world) //DECIDES COL0R ON A RAYTRACE
{
    hit_record rec;
    if (world.hit(ray, 0, INFINITY, rec))
    {
        return 0.5 * (rec.normal + Color(1.0, 1.0, 1.0));
    }

    Vec3 unit_direction = unit_vector(ray.direction());
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
    const int samples = 10;

    //WORLD OBJECTS
    Hittable_List world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    //CAMERA
    Camera cam;

    //WRITING TO PPM FORMAT IMAGE
    ofstream image{ "render.ppm", ios::out | ios::trunc };
    image << "P3\n" << iWidth << ' ' << iHeight << "\n255\n";
    for (int j = iHeight - 1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < iWidth; ++i) 
        {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples; s++)
            {
                auto u = (i + random()) / (iWidth - 1.0);
                auto v = (j + random()) / (iHeight - 1.0);
                Ray ray = cam.get_ray(u, v);
                pixel_color += ray_color(ray, world);
            }
            write_color(image, pixel_color, samples);
        }
    }

    std::cerr << "\nDone.\n";
}