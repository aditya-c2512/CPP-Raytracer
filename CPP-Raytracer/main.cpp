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

Hittable_List random_scene()
{
    Hittable_List world;

    auto MAT_Ground = make_shared<MAT_Lambertian>(Color(0.5, 0.5, 0.5));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random();
            Point3 center(a + 0.9 * random(), 0.2, b + 0.9 * random());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<Material> MAT_Sphere;
                if (choose_mat < 0.8)
                {
                    Color albedo = random_vec() * random_vec();
                    MAT_Sphere = make_shared<MAT_Lambertian>(albedo);

                    world.add(make_shared<Sphere>(center, 0.2, MAT_Sphere));
                }
                else if (choose_mat < 0.95)
                {
                    Color albedo = random_vec(0.5, 1);
                    double fuzz = random(0, 0.5);
                    MAT_Sphere = make_shared<MAT_Metallic>(albedo, fuzz);

                    world.add(make_shared<Sphere>(center, 0.2, MAT_Sphere));
                }
                else
                {
                    MAT_Sphere = make_shared<MAT_Dielectric>(1.5);

                    world.add(make_shared<Sphere>(center, 0.2, MAT_Sphere));
                }
            }
        }
    }

    auto material1 = make_shared<MAT_Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<MAT_Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<MAT_Metallic>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

using namespace std;
int main() 
{
    //IMAGE DIMENSIONS AND CONSTANTS
    auto R = cos(PI / 4);
    const auto aspectRatio = 3.0 / 2.0;
    const int iWidth = 1920;
    const int iHeight = static_cast<int>(iWidth/aspectRatio);
    const int samples = 10;
    const int max_depth = 20;

    //WORLD OBJECTS
    Hittable_List world = random_scene();

    //CAMERA
    Point3 lookFrom(13, 2, 3);
    Point3 lookAt(0, 0, 0);
    Vec3 vUp(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookFrom, lookAt, vUp, 20, aspectRatio, aperture, dist_to_focus);

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