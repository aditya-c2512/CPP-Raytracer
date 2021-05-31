#ifndef SCENES_H
#define SCENES_H

#include "Hittable_List.h"
#include "Sphere.h"
#include "Moving_Sphere.h"
#include "Triangle.h"
#include "AA_Rect.h"
#include "Box.h"
#include "Volume.h"
#include "BVH_Node.h"
#include "Material.h"

Hittable_List triangleTest()
{
    Hittable_List world;
    auto triangle = make_shared<Triangle>(Point3(0,5,-5), Point3(5,0,-5), Point3(0,0,-2), make_shared<MAT_Lambertian>(Color(1, 0, 0)));
    world.add(triangle);
    //auto MAT_Light = make_shared<MAT_Diffuse_Light>(Color(15, 15, 15));
    //world.add(make_shared<XZ_Rect>(-10, 10, -10, 10, 10, MAT_Light));
    return world;
}

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

Hittable_List simple_light()
{
    Hittable_List world;

    auto pertext = make_shared<Perlin_Texture>(4);
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<MAT_Lambertian>(pertext)));
    world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<MAT_Lambertian>(pertext)));

    auto difflight = make_shared<MAT_Diffuse_Light>(Color(4, 4, 4));
    world.add(make_shared<XY_Rect>(3, 5, 1, 3, -2, difflight));
    world.add(make_shared<Sphere>(Point3(0, 6, 0), 2, difflight));

    return world;
}

Hittable_List Cornell_Box()
{
    Hittable_List world;

    auto MAT_Red = make_shared<MAT_Lambertian>(Color(.65, .05, .05));
    auto MAT_Green = make_shared<MAT_Lambertian>(Color(.12, .45, .15));
    auto MAT_White = make_shared<MAT_Lambertian>(Color(.73, .73, .73));
    auto MAT_Light = make_shared<MAT_Diffuse_Light>(Color(15, 15, 15));

    world.add(make_shared<YZ_Rect>(0, 555, 0, 555, 555, MAT_Green));
    world.add(make_shared<YZ_Rect>(0, 555, 0, 555, 0, MAT_Red));
    world.add(make_shared<XZ_Rect>(150, 343, 127, 332, 554, MAT_Light));
    world.add(make_shared<XZ_Rect>(0, 555, 0, 555, 0, MAT_White));
    world.add(make_shared<XZ_Rect>(0, 555, 0, 555, 555, MAT_White));
    world.add(make_shared<XY_Rect>(0, 555, 0, 555, 555, MAT_White));

    shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), MAT_White);
    box1 = make_shared<Rotate_Y>(box1, 45);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
    world.add(make_shared<Volume>(box1,0.01, Color(0,0,0)));

    shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), MAT_White);
    box2 = make_shared<Rotate_Y>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));
    world.add(make_shared<Volume>(box2, 0.01, Color(1, 1, 1)));

    

    return world;
}

Hittable_List final_scene()
{
    Hittable_List boxes1;

    auto MAT_Ground = make_shared<MAT_Lambertian>(Color(0.398, 0.73, 0.83));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++)
    {
        for (int j = 0; j < boxes_per_side; j++)
        {
            auto w = 100.0;
            auto x0 = -1000 + i * w;
            auto y0 = 0.0;
            auto z0 = -1000 + j * w;
            
            auto x1 = x0 + w;
            auto y1 = random(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<Box>(Point3(x0, y0, z0), Point3(x1, y1, z1), MAT_Ground));
        }
    }

    Hittable_List world;

    world.add(make_shared<BVH_Node>(boxes1, 0, 1));

    auto MAT_Light = make_shared<MAT_Diffuse_Light>(Color(7, 7, 7));
    world.add(make_shared<XZ_Rect>(123, 423, 147, 412, 554, MAT_Light));

    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30, 0, 0);
    auto MAT_Moving_Sphere = make_shared<MAT_Lambertian>(Color(0.68, 0.867, 0.269));
    world.add(make_shared<Moving_Sphere>(center1, center2, 0, 1, 50, MAT_Moving_Sphere));

    world.add(make_shared<Sphere>(Point3(260, 150, 45), 50, make_shared<MAT_Dielectric>(1.5)));
    world.add(make_shared<Sphere>(Point3(0, 150, 145), 50, make_shared <MAT_Metallic> (Color(0.746, 0.445, 0.906), 1.0)));

    auto boundary = make_shared<Sphere>(Point3(360, 150, 145), 70, make_shared<MAT_Dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<Volume>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Point3(0, 0, 0), 5000, make_shared<MAT_Dielectric>(1.5));
    world.add(make_shared<Volume>(boundary, .0001, Color(1, 1, 1)));

    auto MAT_Earth = make_shared<MAT_Lambertian>(make_shared<Image_Texture>("earthmap.jpg"));
    world.add(make_shared<Sphere>(Point3(400, 200, 400), 100, MAT_Earth));
    auto pertext = make_shared<Perlin_Texture>(0.1);
    world.add(make_shared<Sphere>(Point3(220, 280, 300), 80, make_shared<MAT_Lambertian>(pertext)));

    Hittable_List boxes2;
    auto MAT_Box2 = make_shared<MAT_Lambertian>(Color(0.836, 0.504, 0.766));
    int ns = 1000;
    for (int j = 0; j < ns; j++)
    {
        boxes2.add(make_shared<Sphere>(random_vec(0, 165), 10, MAT_Box2));
    }

    world.add(make_shared<Translate>(make_shared<Rotate_Y>(make_shared<BVH_Node>(boxes2, 0.0, 1.0), 15), Vec3(-100, 270, 395)));

    return world;
}

#endif