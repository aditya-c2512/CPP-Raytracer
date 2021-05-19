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

	//TEXTURE COORDINATES
	double u; double v;

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

class Translate : public Hittable
{
public :
	Translate(shared_ptr<Hittable> p, const Vec3& off) : ptr(p), offset(off) {}

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& box) const override;

	shared_ptr<Hittable> ptr;
	Vec3 offset;
};
bool Translate::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
	Ray moved_ray(ray.origin() - offset, ray.direction(), ray.time());
	if (!ptr->hit(moved_ray, t_min, t_max, rec)) return false;

	rec.p += offset;
	rec.set_face_normal(moved_ray, rec.normal);

	return true;
}
bool Translate::bounding_box(double time0, double time1, AABB& box) const
{
	if (!ptr->bounding_box(time0, time1, box)) return false;

	box = AABB(box.min() + offset, box.max() + offset);

	return true;
}

class Rotate_Y : public Hittable
{
public :
	Rotate_Y(shared_ptr<Hittable> p, double angle) : ptr(p)
	{
		auto radians = deg_to_rad(angle);
		sin_theta = sin(radians);
		cos_theta = cos(radians);
		hasbox = ptr->bounding_box(0, 1, bbox);

		Point3 min(INFINITY, INFINITY, INFINITY);
		Point3 max(-INFINITY, -INFINITY, -INFINITY);

		for (int i = 0; i < 2; i++) 
		{
			for (int j = 0; j < 2; j++) 
			{
				for (int k = 0; k < 2; k++) 
				{
					auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
					auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
					auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

					auto newx = cos_theta * x + sin_theta * z;
					auto newz = -sin_theta * x + cos_theta * z;

					Vec3 tester(newx, y, newz);

					for (int c = 0; c < 3; c++) 
					{
						min[c] = fmin(min[c], tester[c]);
						max[c] = fmax(max[c], tester[c]);
					}
				}
			}
		}

		bbox = AABB(min, max);
	}

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& box) const override 
	{
		box = bbox;
		return hasbox;
	}

	shared_ptr<Hittable> ptr;
	double sin_theta;
	double cos_theta;
	bool hasbox;
	AABB bbox;
};
bool Rotate_Y::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
	auto origin = ray.origin();
	auto direction = ray.direction();

	origin[0] = cos_theta * ray.origin()[0] - sin_theta * ray.origin()[2];
	origin[2] = sin_theta * ray.origin()[0] + cos_theta * ray.origin()[2];

	direction[0] = cos_theta * ray.direction()[0] - sin_theta * ray.direction()[2];
	direction[2] = sin_theta * ray.direction()[0] + cos_theta * ray.direction()[2];

	Ray rotated_ray(origin, direction, ray.time());
	if (!ptr->hit(rotated_ray, t_min, t_max, rec)) return false;

	auto p = rec.p;
	auto normal = rec.normal;

	p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
	p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

	normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
	normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

	rec.p = p;
	rec.set_face_normal(rotated_ray, normal);

	return true;
}

#endif