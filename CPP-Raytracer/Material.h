#ifndef MATERIAL_H
#define MATERIAL_H

#include "RTMath.h"
#include "Hittable.h"

struct hit_record;

class Material
{
public :
	virtual bool scatter(const Ray& ray_in, const hit_record& rec, Color& attenuation, Ray& scattered) const = 0;
};

class MAT_Lambertian : public Material
{
public :
	MAT_Lambertian(const Color& a) : albedo(a) {}

	virtual bool scatter(const Ray& ray_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override
	{
		auto scatter_direction = rec.normal + random_unit_vector();
		if (scatter_direction.near_zero()) scatter_direction = rec.normal;
		scattered = Ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

	Color albedo;
};
class MAT_Metallic : public Material
{
public :
	MAT_Metallic(const Color& a) : albedo(a) {}

	virtual bool scatter(const Ray& ray_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override
	{
		auto scatter_direction = reflect(unit_vector(ray_in.direction()), rec.normal);
		scattered = Ray(rec.p, scatter_direction);
		attenuation = albedo;
		return (dot(scatter_direction, rec.normal) > 0);
	}

	Color albedo;
};
#endif