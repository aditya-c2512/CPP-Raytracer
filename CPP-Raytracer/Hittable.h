/*
* Hittable.h : Contains main Hittable parent class. Inherit class Hittable to make a shape raytraceable. Also includes hit_record. The raytrace will
*			   store the desired information in this struct type. Material class is forward declared here.
* -> struct hit_record - p is point the ray hits the Hittable. normal is normal vector at that point p. t is constant multiplier for ray(a,b) = a + tb. 
*						 bool front_facing is true if ray hits Hittable from outside and vice versa. set_face_normal(...) sets normal according to front_facing.
*/
#ifndef HITTABLE_H
#define HITTABLE_H

#include "RTMath.h"
#include "AABB.h"

class Material;

struct hit_record
{
	Point3 p;
	Vec3 normal;
	shared_ptr<Material> material;
	double t;
	bool front_facing;

	void set_face_normal(const Ray& ray, const Vec3& outward_normal)
	{
		front_facing = (dot(ray.direction(), outward_normal) < 0.0);
		normal = front_facing ? outward_normal : -outward_normal;
	}
};

class Hittable
{
public :
	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(double time0, double time1, AABB& box) const = 0;
};
#endif