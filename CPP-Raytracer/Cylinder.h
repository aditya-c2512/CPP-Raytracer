#ifndef CYLINDER_H
#define CYLINDER_H

#include "Hittable.h"
#include "Vec3.h"

class Cylinder : public Hittable
{
public :
	Cylinder() : radius(0) {}
	Cylinder(double r, shared_ptr<Material> m) : radius(r), material(m) {}

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;

	double radius;
	shared_ptr<Material> material;
};

bool Cylinder::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
	//x^2 + y^2 = r^2
	//p = a + tb
	//t^2(b.x^2 + b.y^2) + 2t(a.x*b.x + a.y*b.y) + (a.x^2 + a.y^2)-r^2 = 0
	auto a = ray.direction().e[0] * ray.direction().e[0] + ray.direction().e[1] * ray.direction().e[1];
	auto b = 2 * (ray.origin().e[0] * ray.direction().e[0] + ray.origin().e[1] * ray.direction().e[1]);
	auto c = ray.origin().e[0] * ray.origin().e[0] + ray.origin().e[1] * ray.origin().e[1] - radius * radius;
	auto discriminant = b * b - 4 * a * c;

	if (discriminant < 0) return false;

	auto root = (-b - sqrt(discriminant)) / (2 * a);
	if (root < t_min || root > t_max)
	{
		root = (-b + sqrt(discriminant)) / (2 * a);
		if (root < t_min || root > t_max) return false;
	}

	rec.t = root;
	rec.p = ray.at(rec.t);//Finds Point on Sphere which hit the ray.
	rec.material = material;

	return true;
}

#endif
