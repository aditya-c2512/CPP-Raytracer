#ifndef COLOR_H
#define COLOR_H

#include "Vec3.h"
#include <iostream>

void write_color(std::ostream& out, Color pixel_color) //FUNCTION TO SET PIXEL RGB VALUE OF EACH PIXEL
{
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' ' //R
        << static_cast<int>(255.999 * pixel_color.y()) << ' ' //G
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';//B
}

#endif