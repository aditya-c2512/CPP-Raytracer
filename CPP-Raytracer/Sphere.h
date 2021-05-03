#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable
{
public :
	Sphere() : center(Point3(0,0,0)), radius(0) {};
	Sphere(Point3 cen, double r, shared_ptr<Material> m) : center(cen), radius(r), material(m) {};

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;

	Point3 center;
	double radius;
    shared_ptr<Material> material;
};

bool Sphere::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
    Vec3 oc = ray.origin() - center;
    //ray = A +tB
    //C = center
    //t^2*(B.B) + 2t*B.(A-C) + (A-C).(A-C)-r^2 = 0
    auto a = ray.direction().length_squared();
    auto b = dot(oc, ray.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = b * b - a * c; // D = b^2 - 4ac = 4(b/2 ^2 - ac)

    if (discriminant < 0) return false;

    auto root = (-b - sqrt(discriminant)) / a;//Root is value of t, i.e, point along the ray that intersects.
    if (root < t_min || root > t_max)
    {
        root = (-b + sqrt(discriminant)) / a;
        if (root < t_min || root > t_max) return false;
    }
    rec.t = root;
    rec.p = ray.at(rec.t);//Finds Point on Sphere which hit the ray.
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(ray, outward_normal);
    rec.material = material;

    return true;
}

#endif