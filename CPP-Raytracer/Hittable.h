#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "RTMath.h"

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
};
#endif