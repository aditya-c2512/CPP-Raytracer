#ifndef AARECT_H
#define AARECT_H

#include "RTMath.h"
#include "Hittable.h"

class XY_Rect : public Hittable
{
public :
	XY_Rect() {}
	XY_Rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<Material> mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), material(mat) {}

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& box) const override
	{
		box = AABB(Point3(x0, y0, k - 0.0001), Point3(x1, y1, k + 0.0001));
		return true;
	}

	shared_ptr<Material> material;
	double x0, x1, y0, y1, k;
};

bool XY_Rect::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
	auto t = (k - ray.origin().z()) / (ray.direction().z());
	if (t<t_min || t>t_max) return false;

	auto x = ray.origin().x() + t * ray.direction().x();
	auto y = ray.origin().y() + t * ray.direction().y();
	if (x<x0 || x>x1 || y<y0 || y>y1) return false;

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	auto outward_normal = Vec3(0, 0, 1);
	rec.set_face_normal(ray, outward_normal);
	rec.material = material;
	rec.p = ray.at(t);
	return true;
}

class XZ_Rect : public Hittable
{
public:
	XZ_Rect() {}
	XZ_Rect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<Material> mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), material(mat) {}

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& box) const override
	{
		box = AABB(Point3(x0, k - 0.0001, z0), Point3(x1, k + 0.0001, z1));
		return true;
	}

	shared_ptr<Material> material;
	double x0, x1, z0, z1, k;
};

bool XZ_Rect::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
	auto t = (k - ray.origin().y()) / (ray.direction().y());
	if (t<t_min || t>t_max) return false;

	auto x = ray.origin().x() + t * ray.direction().x();
	auto z = ray.origin().z() + t * ray.direction().z();
	if (x<x0 || x>x1 || z<z0 || z>z1) return false;

	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	auto outward_normal = Vec3(0, 1, 0);
	rec.set_face_normal(ray, outward_normal);
	rec.material = material;
	rec.p = ray.at(t);
	return true;
}

class YZ_Rect : public Hittable
{
public:
	YZ_Rect() {}
	YZ_Rect(double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<Material> mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), material(mat) {}

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& box) const override
	{
		box = AABB(Point3(k - 0.0001, y0, z0), Point3(k + 0.0001, y1, z1));
		return true;
	}

	shared_ptr<Material> material;
	double y0, y1, z0, z1, k;
};

bool YZ_Rect::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
	auto t = (k - ray.origin().x()) / (ray.direction().x());
	if (t<t_min || t>t_max) return false;

	auto y = ray.origin().y() + t * ray.direction().y();
	auto z = ray.origin().z() + t * ray.direction().z();
	if (y<y0 || y>y1 || z<z0 || z>z1) return false;

	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	auto outward_normal = Vec3(0, 1, 0);
	rec.set_face_normal(ray, outward_normal);
	rec.material = material;
	rec.p = ray.at(t);
	return true;
}

#endif