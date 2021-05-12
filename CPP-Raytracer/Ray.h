/*
* Ray.h : Contains Ray class
* Please see the comments to know more about each function.
*/
#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray
{
public :
	Ray() {}
	Ray(const Point3& origin, const Vec3& direction, double time = 0.0) : orig(origin), dir(direction), tm(time) {}

	Point3 origin() const //RETURNS ORIGIN OF RAY
	{ return orig; }
	Vec3 direction() const //RETURNS DIRECTION IN WHICH RAY TRAVELS
	{ return dir; }
	double time() const //RETURNS TIME AT WHICH RAY EXISTS
	{ return tm; }
	Point3 at(double t) const //RETURNS POINT ALONG THE RAY
	{
		return orig + dir * t;
	}

	Point3 orig;
	Vec3 dir;
	double tm;
};
#endif
