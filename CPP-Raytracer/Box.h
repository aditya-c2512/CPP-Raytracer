#ifndef BOX_H
#define BOX_H

#include "AA_Rect.h"
#include "Hittable_List.h"

class Box : public Hittable
{
public :
	Box() {}
	Box(const Point3& p0, const Point3& p1, shared_ptr<Material> mat)
	{
		box_min = p0;
		box_max = p1;

		sides.add(make_shared<XY_Rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat));
		sides.add(make_shared<XY_Rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat));
																			   
		sides.add(make_shared<XZ_Rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat));
		sides.add(make_shared<XZ_Rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat));
																			   
		sides.add(make_shared<YZ_Rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat));
		sides.add(make_shared<YZ_Rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat));
	}

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& box) const override
	{
		box = AABB(box_min, box_max);
		return true;
	}

	Point3 box_min;
	Point3 box_max;
	Hittable_List sides;
};

bool Box::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
	return sides.hit(ray, t_min, t_max, rec);
}

#endif