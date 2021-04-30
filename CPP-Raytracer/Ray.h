#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray
{
public :
	Ray() {}
	Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

	Point3 origin() const //RETURNS ORIGIN OF RAY
	{ return orig; }
	Vec3 direction() const //RETURNS DIRECTION IN WHICH RAY TRAVELS
	{ return dir; }
	Point3 at(double t) const //RETURNS POINT ALONG THE RAY
	{
		return orig + dir * t;
	}

	Point3 orig;
	Vec3 dir;
};
#endif