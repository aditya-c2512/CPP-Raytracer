/*
* RTMath.h : Utility class for all functions, values and basic files needed by other classes and functions.
* -> Include this file if you need to use - Vec3.h, Ray.h, limits, or math functions.
* -> Also includes - Degree to Radians, Random between 0 and 1, Random between 2 numbers, Clamping, values for Infinity and PI.
*/
#ifndef RTMATH_H
#define RTMATH_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<float>::infinity();
const double PI = 3.1415926535897932385;

inline double deg_to_rad(double deg)
{
	return deg * PI / 180.0;
}

double random()
{
	static std::uniform_real_distribution<double> dist(0.0, 1.0);
	static std::mt19937 engine;
	return dist(engine);
}
inline double random(double min, double max)
{
	return min + (max - min) * random();
}

double clamp(double x, double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

#include "Ray.h"
#include "Vec3.h"
#endif
