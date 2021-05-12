#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "RTMath.h"
#include "Hittable.h"

class Moving_Sphere : public Hittable
{
public :
	Moving_Sphere() {};
	Moving_Sphere(Point3 cen0, Point3 cen1, double t0, double t1, double r, shared_ptr<Material> mat) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), material(mat) {};

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& box) const override;

	Point3 center(double time) const;

	Point3 center0, center1;
	double time0, time1;
	double radius;
	shared_ptr<Material> material;
};

Point3 Moving_Sphere::center(double time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool Moving_Sphere::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const //SEE ATTACHED NOTES FOR FIGURES AND EXPLANATIONS
{
    Vec3 oc = ray.origin() - center(ray.time());
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
    //std::cerr << "Sphere Hit" << std::flush;
    rec.t = root;
    rec.p = ray.at(rec.t);//Finds Point on Sphere which hit the ray.
    Vec3 outward_normal = (rec.p - center(ray.time())) / radius;
    rec.set_face_normal(ray, outward_normal);
    rec.material = material;

    return true;
}

bool Moving_Sphere::bounding_box(double time0, double time1, AABB& box) const
{
    AABB box0 = AABB(center(time0) - Vec3(radius, radius, radius), center(time0) + Vec3(radius, radius, radius));
    AABB box1 = AABB(center(time1) - Vec3(radius, radius, radius), center(time1) + Vec3(radius, radius, radius));

    box = surrounding_box(box0, box1);
    return true;
}

#endif