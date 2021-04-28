#ifndef VEC3_H
#define VEC3_H

#include <iostream>

using namespace std;
class Vec3
{
public :
	Vec3();
	Vec3(double e1, double e2, double e3);

	double x() const;
	double y() const;
	double z() const;

	Vec3 operator-() const;
	double operator[](int i) const;
	double& operator[](int i);
	Vec3& operator+=(const Vec3& v);
	Vec3& operator*=(const double t);
	Vec3& operator/=(const double t);

    double length() const;
    double length_squared() const;

	double e[3];
};

using Point3 = Vec3; // 3D Point Alias
using Color = Vec3; // RGB Color Alias

#endif