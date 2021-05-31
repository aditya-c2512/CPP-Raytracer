#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "RTMath.h"
#include "Hittable.h"

class Triangle : public Hittable
{
public :
	Triangle() {}
	Triangle(Point3 aa, Point3 bb, Point3 cc, shared_ptr<Material> mat) : a(aa), b(bb), c(cc), material(mat) {}

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& box) const override
	{
		return true;
	}

	Point3 a;
	Point3 b;
	Point3 c;

	shared_ptr<Material> material;
};
bool Triangle::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
	Vec3 edge01 = b - a;
	Vec3 edge02 = c - a;

	Vec3 n = unit_vector(cross(edge01, edge02));
	Vec3 q = cross(ray.direction(), edge02);
	double x = dot(edge01, q);

	if ((dot(n, ray.direction()) >= 0) || fabs(x) <= 1e-6) return false;

	Vec3 s = (ray.origin() - a) / x;
	Vec3 r = cross(s, edge01);

	double bary[3];
	bary[0] = dot(s, q);
	bary[1] = dot(r, ray.direction());
	bary[2] = 1.0f - b[0] - b[1];

	if ((bary[0] < 0.0f) || (bary[1] < 0.0f) || (bary[2] < 0.0f)) return false;

	double t = dot(edge02, r);
	if (t < t_min || t > t_max) return false;

	rec.t = t;
	rec.p = ray.at(rec.t);
	rec.normal = n;
	rec.u = 0; rec.v = 0;
	rec.material = material;
	std::cerr << "Triangle Hit" << std::flush;
	return true;
}
#endif