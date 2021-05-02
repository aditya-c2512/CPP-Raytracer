#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"
#include <iostream>

void write_color(std::ostream& out, Color pixel_color, int samples) //FUNCTION TO SET PIXEL RGB VALUE OF EACH PIXEL
{
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    auto scale = 1.0 / samples;
    //GAMMA CORRECTED RGB VALUES
    r = sqrt(r * scale);
    g = sqrt(r * scale);
    b = sqrt(r * scale);

    //std::cout << r << " " << g << " " << b << endl;

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0, 0.99)) << ' ' //R
        << static_cast<int>(256 * clamp(g, 0, 0.99)) << ' ' //G
        << static_cast<int>(256 * clamp(b, 0, 0.99)) << '\n';//B
}

#endif