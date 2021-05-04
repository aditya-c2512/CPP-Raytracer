#include "RTMath.h"

#include "Color.h"
#include "Hittable_List.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include <iostream>
#include <fstream>

Color ray_color(const Ray& ray, const Hittable& world, int depth) //DECIDES COL0R ON A RAYTRACE
{
    hit_record rec;
    if (depth <= 0)return Color(0, 0, 0);//DEPTH TESTING

    if (world.hit(ray, 0.001, INFINITY, rec))
    {
        Color attenuation;
        Ray scattered;
        if (rec.material->scatter(ray, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return Color(0, 0, 0);
    }

    //IF RAYS DONT HIT A SPHERE, THIS COLOR IS RETURNED
    Vec3 unit_direction = unit_vector(ray.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

using namespace std;
int main() 
{
    //IMAGE DIMENSIONS AND CONSTANTS
    const auto aspectRatio = 16.0 / 9.0;
    const int iWidth = 1920;
    const int iHeight = static_cast<int>(iWidth/aspectRatio);
    const int samples = 20;
    const int max_depth = 50;

    //WORLD OBJECTS
    Hittable_List world;

    auto MAT_Ground = make_shared<MAT_Lambertian>(Color(0.1, 0.5, 0.1));
    auto MAT_CenterSphere = make_shared<MAT_Lambertian>(Color(0.6, 0.4, 0.7));
    auto MAT_LeftSphere = make_shared<MAT_Dielectric>(1.5);
    auto MAT_RightSphere = make_shared<MAT_Metallic>(Color(0.8, 0.6, 0.2));

    world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, MAT_Ground));
    world.add(make_shared<Sphere>(Point3(0.0, -0.0, -1.0), 0.5, MAT_LeftSphere));
    world.add(make_shared<Sphere>(Point3(0.0, -0.0, -1.0), -0.4, MAT_LeftSphere));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, MAT_LeftSphere));
    world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, MAT_RightSphere));

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
                pixel_color += ray_color(ray, world, 50);
            }
            write_color(image, pixel_color, samples);
        }
    }

    std::cerr << "\nDone.\n";
}