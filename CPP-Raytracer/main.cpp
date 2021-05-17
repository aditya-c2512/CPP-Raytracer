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

#include <thread>
#include <mutex>

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

void raytrace(int* nextTileX, int* nextTileY, mutex* tileInfoMutex, int tileWidth, int tileHeight, int nx, int ny, int ns, int depth, Hittable_List world, Camera cam, unsigned char* data)
{
    while (true)
    {
        tileInfoMutex->lock();

        int tileX = *nextTileX;
        int tileY = *nextTileY;

        *nextTileX += tileWidth;

        if (*nextTileX >= nx)
        {
            *nextTileX = 0;
            *nextTileY += tileHeight;
        }

        tileInfoMutex->unlock();

        if (tileY >= ny) return;

        // Clip tile to screen borders
        tileWidth = tileWidth - max(0, tileX + tileWidth - nx);
        tileHeight = tileHeight - max(0, tileY + tileHeight - ny);

        for (int j = tileY; j < tileY + tileHeight; j++)
        {
            for (int i = tileX; i < tileX + tileWidth; i++)
            {
                Color color(0, 0, 0);
                for (int s = 0; s < ns; s++)
                {
                    auto u = (i + random()) / (nx - 0.0);
                    auto v = (j + random()) / (ny - 0.0);

                    Ray ray = cam.get_ray(u, v);
                    color += ray_color(ray, world, depth);
                }
                color /= double(ns);
                color = Color(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

                double r = 255.99 * color[0];
                double g = 255.99 * color[1];
                double b = 255.99 * color[2];

                int bufferIdx = (i + ((ny - 1) - j) * nx) * 3;

                data[bufferIdx] = (unsigned char)(256 * clamp(r, 0, 0.99));
                data[bufferIdx+1] = (unsigned char)(256 * clamp(g, 0, 0.99));
                data[bufferIdx+2] = (unsigned char)(256 * clamp(b, 0, 0.99));
            }
        }
    }
}

using namespace std;
int main() 
{
    //THREAD DATA
    int tileWidth = 32;
    int tileHeight = 32;
    unsigned int threadsNum = (thread::hardware_concurrency() > 1) ? thread::hardware_concurrency() : 1;
    thread* threads = new thread[threadsNum];
    int nextTileX = 0;
    int nextTileY = 0;
    mutex tileInfoMutex;

    //IMAGE DIMENSIONS AND CONSTANTS
    const auto aspectRatio = 16.0 / 9.0;
    const int iWidth = 1920;
    const int iHeight = static_cast<int>(iWidth/aspectRatio);
    const int samples = 5;//INCREASE FOR LESS NOISE
    const int max_depth = 1;

    //WORLD OBJECTS
    Hittable_List world = random_scene();

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

    /*for (int j = iHeight - 1; j >= 0; --j) 
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
    }*/

    for (unsigned t = 0; t < threadsNum; t++)
    {
        threads[t] = thread(raytrace, &nextTileX, &nextTileY, &tileInfoMutex, tileWidth, tileHeight, iWidth, iHeight, samples, max_depth, world, cam, data);
    }

    for (unsigned int t = 0; t < threadsNum; t++) 
    {
        threads[t].join();
    }

    stbi_write_png("render.png", iWidth, iHeight, 3, data, iWidth * 3);
    std::cerr << "\nDone.\n";
    delete[] data;
}