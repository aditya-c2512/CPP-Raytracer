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
	MAT_Metallic(const Color& a, double f) : albedo(a), fuzz(f) {}

	virtual bool scatter(const Ray& ray_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override
	{
		auto scatter_direction = reflect(unit_vector(ray_in.direction()), rec.normal);
		scattered = Ray(rec.p, scatter_direction + fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scatter_direction, rec.normal) > 0);
	}

	Color albedo;
	double fuzz;
};

class MAT_Dielectric : public Material
{
public :
	MAT_Dielectric(double index_of_refraction) : mu(index_of_refraction) {}

	virtual bool scatter(const Ray& ray_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_facing ? (1.0 / mu) : mu;
		Vec3 unit_direction = unit_vector(ray_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;
		Vec3 direction;

		if (cannot_refract || reflectance(cos_theta,refraction_ratio)>random())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = Ray(rec.p, direction);
		return true;
	}

	double mu;

private :
	static double reflectance(double cosine, double ref_idx)
	{
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};
#endif