#include "Color.h"
#include "Vec3.h"
#include <iostream>
#include <fstream>

using namespace std;
int main() 
{
    const int iWidth = 1024;
    const int iHeight = 1024;

    ofstream image{ "render.ppm", ios::out | ios::trunc };
    image << "P3\n" << iWidth << ' ' << iHeight << "\n255\n";
    for (int j = iHeight - 1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < iWidth; ++i) 
        {
            Color pixel_color(double(j) / (iHeight - 1), double(i) / (iWidth - 1), 0.35);
            write_color(image, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}