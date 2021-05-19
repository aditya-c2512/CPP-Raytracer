#ifndef VOLUME_H
#define VOLUME_H

#include "RTMath.h"
#include "Hittable.h"
#include "Material.h"
#include "Texture.h"

class Volume : public Hittable
{
public :
	Volume(shared_ptr<Hittable> b, double d, shared_ptr<Texture> a) : boundary(b), neg_inv_density(-1/d), phase_function(make_shared<MAT_Isotropic>(a)) {}
	Volume(shared_ptr<Hittable> b, double d, Color a) : boundary(b), neg_inv_density(-1/d), phase_function(make_shared<MAT_Isotropic>(a)) {}

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& box) const override
	{
		return boundary->bounding_box(time0, time1, box);
	}

	shared_ptr<Hittable> boundary;
	shared_ptr<Material> phase_function;
	double neg_inv_density;
};

bool Volume::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
	bool enableDebug = false;
	const bool debugging = enableDebug && (random() < 0.00001);

	hit_record rec1, rec2;
	if (!boundary->hit(ray, -INFINITY, INFINITY, rec1)) return false;
	if (!boundary->hit(ray, rec1.t + 0.00001, INFINITY, rec2)) return false;

	if (debugging) std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';

	if (rec1.t < t_min) rec1.t = t_min;
	if (rec2.t > t_max) rec2.t = t_max;

	if (rec1.t >= rec2.t) return false;

	if (rec1.t < 0) rec1.t = 0;

	const auto ray_length = ray.direction().length();
	const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
	const auto hit_distance = neg_inv_density * log(random());

	if (hit_distance > distance_inside_boundary) return false;

	rec.t = rec1.t + hit_distance / ray_length;
	rec.p = ray.at(rec.t);

	if (debugging) 
	{
		std::cerr << "hit_distance = " << hit_distance << '\n'
				  << "rec.t = " << rec.t << '\n'
				  << "rec.p = " << rec.p << '\n';
	}

	rec.normal = Vec3(1, 0, 0);  // arbitrary
	rec.front_facing = true;
	rec.material = phase_function;

	return true;
}

#endif