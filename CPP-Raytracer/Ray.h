#ifndef RAY_H
#define RAY_H

#include "Vec3.h"
class Ray
{
public :
	Ray() {}
	Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

	Point3 origin()
	{ return orig; }
	Vec3 direction()
	{ return dir; }
	Point3 at(double t) const
	{
		return orig + dir * t;
	}

	Point3 orig;
	Vec3 dir;
};
#endif