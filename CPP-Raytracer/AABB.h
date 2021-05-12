/*
* AABB.h : This class represents an Axis-Aligned Bounding Box in the world.
*/
#ifndef AABB_H
#define AABB_H

#include "RTMath.h"

class AABB
{
public :
	AABB() {}
	AABB(Point3 min, Point3 max) : minimum(min), maximum(max) {}

	Point3 min() const { return minimum; }
	Point3 max() const { return maximum; }

	bool hit(const Ray& ray, double t_min, double t_max) const
	{
		for (int a = 0; a < 3; a++)
		{
			auto invD = 1.0f / ray.direction()[a];
			auto t0 = (min()[a] - ray.origin()[a]) * invD;
			auto t1 = (max()[a] - ray.origin()[a]) * invD;

			if (invD < 0.0f) std::swap(t0, t1);
			
			t_min = fmax(t_min, t0);
			t_max = fmin(t_max, t1);

			if (t_max <= t_min) return false;
		}

		return true;
	}

	Point3 minimum;
	Point3 maximum;
};

AABB surrounding_box(AABB box0, AABB box1)
{
	Point3 small(fmin(box0.min().x(), box1.min().x()),
		fmin(box0.min().y(), box1.min().y()),
		fmin(box0.min().z(), box1.min().z()));

	Point3 big(fmax(box0.max().x(), box1.max().x()),
		fmax(box0.max().y(), box1.max().y()),
		fmax(box0.max().z(), box1.max().z()));

	return AABB(small, big);
}

#endif
