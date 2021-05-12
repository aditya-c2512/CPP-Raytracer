/*
* Material.h : Contains the parent class Material, inherited by classes made for Diffuse, Metallic, and Translucent materials.
* -> A Material type must override the scatter(...) function to be successfully rendered.
* -> SEE NOTES for figures and mathematics behind each scatter function behaviour.
*/
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

class MAT_Lambertian : public Material //CLASS FOT DIFFUSE MATERIALS
{
public :
	MAT_Lambertian(const Color& a) : albedo(a) {}

	virtual bool scatter(const Ray& ray_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override
	{
		/*
		* Diffuse materials only follow laws of reflection on a micro scale. At a macro scale, the surface seems to randomly reflect light.
		* Below, the scatter direction is randomly allocated around the surface normal in a hemisphere.
		*/
		auto scatter_direction = rec.normal + random_unit_vector();
		if (scatter_direction.near_zero()) scatter_direction = rec.normal;
		scattered = Ray(rec.p, scatter_direction, ray_in.time());
		attenuation = albedo;
		return true;
	}

	Color albedo;
};

class MAT_Metallic : public Material //CLASS FOR METALLIC MATERIALS
{
public :
	MAT_Metallic(const Color& a, double f) : albedo(a), fuzz(f) {}

	virtual bool scatter(const Ray& ray_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override
	{
		/*
		* Perfect Metallic materials follow laws of reflection on a macro scale too.
		* Below, the scatter direction is the reflection vector of the ray direction vector about the normal vector.
		*/
		auto scatter_direction = reflect(unit_vector(ray_in.direction()), rec.normal);
		scattered = Ray(rec.p, scatter_direction + fuzz*random_in_unit_sphere(), ray_in.time());
		attenuation = albedo;
		return (dot(scatter_direction, rec.normal) > 0);
	}

	Color albedo;
	double fuzz;
};

class MAT_Dielectric : public Material //CLASS FOR DIELECTRIC/TRANSLUCENT MATERIALS
{
public :
	MAT_Dielectric(double index_of_refraction) : mu(index_of_refraction) {}

	virtual bool scatter(const Ray& ray_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override
	{
		/*
		* See Notes for full explanation.
		* Snell's Law is followed and Total Internal Reflection is added if cannot_refract is true.
		*/
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

		scattered = Ray(rec.p, direction, ray_in.time());
		return true;
	}

	double mu;

private :
	static double reflectance(double cosine, double ref_idx)
	{
		//Schlick's Approximation for reflectance
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};
#endif