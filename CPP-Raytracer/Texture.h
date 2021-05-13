#ifndef TEXTURE_H
#define TEXTURE_H

#include "RTMath.h"

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

#endif