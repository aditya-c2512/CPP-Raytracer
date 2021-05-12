/*
* Plane.h : This is the user defined data type that represents a plane in the world.
* -> See the comments to learn more about each function and variable.
*/
#ifndef PLANE_H
#define PLANE_H

#include "Hittable.h"
#include "RTMath.h"

class Plane : public Hittable
{
public :
	Plane() {};
	Plane(Vec3 normal, Point3 point, shared_ptr<Material> mat) : n(normal), pt(point), material(mat) {};

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& box) const override;

	Vec3 normal() const
	{ return n; }
	Point3 point() const
	{ return pt; }

	Vec3 n;
	Point3 pt;
	shared_ptr<Material> material;
};

bool Plane::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const //SEE ATTACHED NOTES FOR FIGURES AND EXPLANATIONS
{
	//For a Ray(a+tb) to intersect a plane with normal n and point p,
	//t=((p-a).n)/(b.n)
	Vec3 w = point() - ray.origin();
	double root = dot(w, -unit_vector(normal())) / dot(unit_vector(ray.direction()), -unit_vector(normal()));

	if (dot(ray.direction(), normal()) <= 1e-6 || root<t_min || root>t_max) return false;
	//std::cerr << "Plane Hit" << std::flush;
	rec.t = root;
	rec.p = ray.at(rec.t);
	rec.normal = unit_vector(normal());
	rec.material = material;

	return true;
}

bool Plane::bounding_box(double time0, double time1, AABB& box) const
{
	return false;
}
#endif