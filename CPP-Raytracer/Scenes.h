#ifndef SCENES_H
#define SCENES_H

#include "Hittable_List.h"
#include "Sphere.h"
#include "Moving_Sphere.h"
#include "Plane.h"
#include "Material.h"

Hittable_List two_spheres()
{
    Hittable_List objects;

    auto checker = make_shared<Checker_Texture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    objects.add(make_shared<Sphere>(Point3(0, -10, 0), 10, make_shared<MAT_Lambertian>(checker)));
    objects.add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<MAT_Lambertian>(checker)));

    return objects;
}


Hittable_List random_scene() //RETURNS A RANDOM HITTABLE LIST
{
    Hittable_List world;

    auto T_Checker = make_shared<Checker_Texture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    auto MAT_Ground = make_shared<MAT_Lambertian>(T_Checker);
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, MAT_Ground));

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

                    auto center2 = center + Vec3(0, random(0, 0.5), 0);

                    world.add(make_shared<Moving_Sphere>(center, center2, 0.0, 1.0, 0.2, MAT_Sphere));
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

Hittable_List two_perlin_spheres() 
{
    Hittable_List objects;

    auto pertext = make_shared<Perlin_Texture>(4);
    objects.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<MAT_Lambertian>(pertext)));
    objects.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<MAT_Lambertian>(pertext)));

    return objects;
}

Hittable_List earth()
{
    Hittable_List world;

    auto TEX_Earth = make_shared<Image_Texture>("earthmap.jpg");
    auto MAT_Earth = make_shared<MAT_Lambertian>(TEX_Earth);
    auto earth = make_shared<Sphere>(Point3(0, 0, 0), 2, MAT_Earth);

    world.add(earth);
    return world;
}

#endif