#ifndef TEXTURE_H
#define TEXTURE_H

#include "RTMath.h"
#include "Perlin.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

class Texture
{
public :
	virtual Color value(double u, double v, const Point3& p) const = 0;
};

class Solid_Color : public Texture
{
public :
	Solid_Color() {}
	Solid_Color(Color c) : color(c) {}
	Solid_Color(double red, double green, double blue) : Solid_Color(Color(red,green,blue)) {}

	virtual Color value(double u, double v, const Point3& p) const override
	{
		return color;
	}
private :
	Color color;
};

class Checker_Texture : public Texture
{
public :
	Checker_Texture() {}
	Checker_Texture(shared_ptr<Texture> e, shared_ptr<Texture> o) : even(o), odd(o) {}
	Checker_Texture(Color c1, Color c2) : even(make_shared<Solid_Color>(c1)), odd(make_shared<Solid_Color>(c2)) {}

	virtual Color value(double u, double v, const Point3& p) const override
	{
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());

		if (sines < 0) return odd->value(u, v, p);
		else return even->value(u, v, p);
	}

private :
	shared_ptr<Texture> even;
	shared_ptr<Texture> odd;
};

class Perlin_Texture : public Texture
{
public :
	Perlin_Texture() {}
	Perlin_Texture(double sc) : scale(sc) {}

	virtual Color value(double u, double v, const Point3& p) const override
	{
		return Color(1, 1, 1) * 0.5 * (1.0 + sin(scale * p.z() + 10 * noise.turbulence(p)));
	}

private :
	Perlin noise;
	double scale;
};

class Image_Texture : public Texture
{
public :
	const static int bytes_per_pixel = 3;

	Image_Texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

	Image_Texture(const char* filename)
	{
		auto components_per_pixel = bytes_per_pixel;

		data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

		if (!data) 
		{
			std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
			width = height = 0;
		}

		bytes_per_scanline = bytes_per_pixel * width;
	}

	~Image_Texture() 
	{
		delete data;
	}

	virtual Color value(double u, double v, const Point3& p) const override
	{
		if (data == nullptr) return Color(0, 1, 1);

		u = clamp(u, 0.0, 1.0);
		v = 1.0 - clamp(v, 0.0, 1.0);
		auto i = static_cast<int>(u * width);
		auto j = static_cast<int>(v * height);

		if (i >= width)  i = width - 1;
		if (j >= height) j = height - 1;

		const auto color_scale = 1.0 / 255.0;
		auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

		return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
	}

private :
	unsigned char* data;
	int width, height;
	int bytes_per_scanline;
};

#endif