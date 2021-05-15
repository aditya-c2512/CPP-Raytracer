/*
* main.cpp : MAIN EXECUTABLE CODE FOR RENDERER
* -> main() - function defines many image properties and quality properties and initialiazes camera, world, and writes rendered framebuffer to render.ppm
* -> ray_color(...) - This function returns the color traced/calculated by the ray thrown from camera through the pixel. RECURSIVE IN NATURE.
* -> random_scene() - This function returns a list of Hittable objects spawned randomly/procedurally.
*/
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "RTMath.h"
#include "Color.h"
#include "Hittable_List.h"
#include "Camera.h"
#include "Scenes.h"

#include <iostream>
#include <fstream>
#include <vector>

Color ray_color(const Ray& ray, const Hittable& world, int depth) //DECIDES COL0R ON A RAYTRACE
{
    hit_record rec;//STORES IMPORTANT INFO OF THE CLOSEST OBJECT HIT BY A RAY
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
    const int samples = 10;//INCREASE FOR LESS NOISE
    const int max_depth = 5;

    //WORLD OBJECTS
    Hittable_List world = earth();

    //CAMERA
    Point3 lookFrom(13, 2, 3);
    Point3 lookAt(0, 0, 0);
    Vec3 vUp(0, 1, 0);
    double vFOV = 40;
    auto dist_to_focus = 10.0;//DISTANCE FROM FOCUS PLANE
    auto aperture = 0.1;//DIAMETER OF APERTURE

    Camera cam(lookFrom, lookAt, vUp, vFOV, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);

    //WRITING TO FRAMEBUFFER
    vector<vector<Color>> frameBuffer(iHeight, vector<Color>(iWidth));
    unsigned char* data = new unsigned char[iWidth*iHeight*3];

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
                Ray ray = cam.get_ray(u, v);// Getting ray originating from camera through the pixel with coords (u,v).
                pixel_color += ray_color(ray, world, 50);// Getting Color the ray results in at the end.
            }
            frameBuffer[iHeight - 1 - j][i] = pixel_color;
        }
    }
    
    int index = 0;
    //WRITING FRAMEBUFFER TO RENDER.PNG
    for (int i = 0; i < iHeight; i++)
    {
        for (int j = 0; j < iWidth; j++)
        {
            auto scale = 1.0 / samples;
            double r = sqrt(frameBuffer[i][j].x() * scale);
            double g = sqrt(frameBuffer[i][j].y() * scale);
            double b = sqrt(frameBuffer[i][j].z() * scale);

            data[index++] = (unsigned char)(256 * clamp(r, 0, 0.99));
            data[index++] = (unsigned char)(256 * clamp(g, 0, 0.99));
            data[index++] = (unsigned char)(256 * clamp(b, 0, 0.99));
        }
    }
    stbi_write_png("render.png", iWidth, iHeight, 3, data, iWidth * 3);
    std::cerr << "\nDone.\n";
}