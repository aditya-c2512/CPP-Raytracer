#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>
#include "RTMath.h"

using namespace std;

class Vec3
{
public:
	Vec3() : e{ 0,0,0 } {}
	Vec3(double e1, double e2, double e3) : e{ e1,e2,e3 } {}

	double x() const
	{
		return e[0];
	}
	double y() const
	{
		return e[1];
	}
	double z() const
	{
		return e[2];
	}

	Vec3 operator-() const
	{
		return Vec3(-e[0], -e[1], -e[2]);
	}
	double operator[](int i) const
	{
		return e[i];
	}
	double& operator[](int i)
	{
		return e[i];
	}
	Vec3& operator+=(const Vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	Vec3& operator*=(const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	Vec3& operator/=(const double t)
	{
		return *this *= 1 / t;
	}

	double length() const //RETURNS LENGTH OF VECTOR
	{
		return sqrt(length_squared());
	}
	double length_squared() const //RETURNS NORM OF VECTOR
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	bool near_zero() const
	{
		const auto s = 1e-8;
		return (fabs(e[0]) < s && (fabs(e[1]) < s) && (fabs(e[2]) < s));
	}

	double e[3];
};

using Point3 = Vec3; // 3D Point Alias
using Color = Vec3; // RGB Color Alias

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) 
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) 
{
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) 
{
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) 
{
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3& v) 
{
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3& v, double t) 
{
	return t * v;
}

inline Vec3 operator/(Vec3 v, double t) 
{
	return (1 / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v) //RETURNS U.V
{
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) // RETURNS U X V
{
	return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 unit_vector(Vec3 v) //RETURNS UNIT VECTOR OF PARAMETER
{
	return v / v.length();
}

//RANDOM VECTOR GENERATION FOR REFLECTION
inline Vec3 random_vec()
{
	return Vec3(random(), random(), random());
}
inline Vec3 random_vec(double min, double max)
{
	return Vec3(random(min, max), random(min, max), random(min, max));
}
Vec3 random_in_unit_sphere()//RETURNS VECTOR OF LENGTH 1
{
	while (true)
	{
		Vec3 p = random_vec(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}
Vec3 random_unit_vector() //INCORRECT LAMBERTIAN DISTRIBUTION
{
	return unit_vector(random_in_unit_sphere());
}
Vec3 random_vec_hemisphere(const Vec3& normal) //TRUE LAMBERTIAN DISTRIBUTION
{
	Vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) return in_unit_sphere;
	else return -in_unit_sphere;
}

//REFLECTION
Vec3 reflect(const Vec3& v, const Vec3& n)
{
	return v - 2 * dot(v, n) * n;
}
//REFRACTION
Vec3 refract(const Vec3& uv, const Vec3& n, double mu)
{
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	auto ray_out_perp = mu * (uv + cos_theta * n);
	auto ray_out_parallel = -sqrt(fabs(1 - ray_out_perp.length_squared())) * n;

	return ray_out_perp + ray_out_parallel;
}

#endif