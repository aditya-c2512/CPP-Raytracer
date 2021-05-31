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

long int pixelsDone = 0;

Color ray_color(const Ray& ray, const Color& background,const Hittable& world, int depth) //DECIDES COL0R ON A RAYTRACE
{
    hit_record rec;//STORES IMPORTANT INFO OF THE CLOSEST OBJECT HIT BY A RAY
    if (depth <= 0)return Color(0, 0, 0);//DEPTH TESTING

    if (!world.hit(ray, 0.001, INFINITY, rec))
        return background;

    Ray scattered;
    Color attenuation;
    Color emitted = rec.material->emitted(rec.u, rec.v, rec.p);

    if (!rec.material->scatter(ray, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

void raytrace(int* nextTileX, int* nextTileY, std::mutex* tileInfoMutex, int tileWidth, int tileHeight, int nx, int ny, int ns, int depth, Color background, Hittable_List world, Camera cam, char* data)
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
                    color += ray_color(ray, background, world, depth);
                }
                color /= double(ns);
                color = Color(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

                int r = (int)(255 * color[0]);
                int g = (int)(255 * color[1]);
                int b = (int)(255 * color[2]);

                int bufferIdx = (i + ((ny - 1) - j) * nx) * 3;

                data[bufferIdx + 0] = clamp(r, 0, 255);
                data[bufferIdx + 1] = clamp(g, 0, 255);
                data[bufferIdx + 2] = clamp(b, 0, 255);

                std::cerr << (pixelsDone++) << " " << std::flush;
            }
        }
    }

    std::cerr << "Thread Done" << std::flush;
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
    const auto aspectRatio = 1.0;
    const int iWidth = 200;
    const int iHeight = static_cast<int>(iWidth/aspectRatio);
    const int samples = 1;//INCREASE FOR LESS NOISE
    const int max_depth = 5;

    //WORLD OBJECTS
    Color background(0, 01, 0);
    Hittable_List world = triangleTest();

    //CAMERA
    Point3 lookFrom(0, 0, 0);
    Point3 lookAt(0, 0, -5);
    Vec3 vUp(0, 1, 0);
    double vFOV = 40;
    auto dist_to_focus = 1000.0;//DISTANCE FROM FOCUS PLANE
    auto aperture = 0.1;//DIAMETER OF APERTURE

    Camera cam(lookFrom, lookAt, vUp, vFOV, aspectRatio, aperture, dist_to_focus, 0.0, 1.0);

    //WRITING TO FRAMEBUFFER
    char* data = new char[iWidth*iHeight*3];

    std::cerr << threadsNum << std::flush;
    for (unsigned t = 0; t < threadsNum; t++)
    {
        threads[t] = thread(raytrace, &nextTileX, &nextTileY, &tileInfoMutex, tileWidth, tileHeight, iWidth, iHeight, samples, max_depth, background, world, cam, data);
    }

    for (unsigned int t = 0; t < threadsNum; t++)
    {
        threads[t].join();
    }

    stbi_write_png("render.png", iWidth, iHeight, 3, data, iWidth * 3);
    std::cerr << "\nDone.\n";
    delete[] data;
}