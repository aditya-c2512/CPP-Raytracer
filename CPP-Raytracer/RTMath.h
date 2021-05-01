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

double deg_to_rad(double deg)
{
	return deg * PI / 180.0;
}

double random()
{
	static std::uniform_real_distribution<double> dist(0.0, 1.0);
	static std::mt19937 engine;
	return dist(engine);
}
double random(double min, double max)
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
