/*
* BVH_Node.h : This class is the Node of the Bounding Volume Hierarchy Tree. It has an Axis-Aligned Bounding Box and pointers to the 2 children named
*			   conventionally as left and right. 
*/
#ifndef BVH_NODE_H
#define BVH_NODE_H

#include <algorithm>
#include "RTMath.h"
#include "Hittable.h"
#include "Hittable_List.h"

class BVH_Node : public Hittable
{
public :
	BVH_Node() {}
	BVH_Node(const Hittable_List& list, double time0, double time1) : BVH_Node(list.objects, 0, list.objects.size(), time0, time1) {}
	BVH_Node(const std::vector<shared_ptr<Hittable>>& src_objects, size_t start, size_t end, double time0, double time1)
	{
		auto objects = src_objects;
		int axis = random_int(0, 2);
		auto comparator = (axis == 0) ? &BVH_Node::box_x_compare : (axis == 1) ? &BVH_Node::box_y_compare : &BVH_Node::box_z_compare;

		size_t object_span = end - start;

		if (object_span == 1) left = right = objects[start];
		else if (object_span == 2)
		{
			if (box_compare(objects[start], objects[start + 1], axis))
			{
				left = objects[start]; right = objects[start + 1];
			}
			else
			{
				left = objects[start+1]; right = objects[start];
			}
		}
		else
		{
			std::sort(objects.begin() + start, objects.begin() + end, comparator);
			auto mid = start + object_span / 2;
			left = make_shared<BVH_Node>(objects, start, mid, time0, time1);
			right = make_shared<BVH_Node>(objects, mid, end, time0, time1);
		}

		AABB box_left, box_right;

		if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
			std::cerr << "NO BOUNDING BOX IN BVH NODE\n";

		BVH_Box = surrounding_box(box_left, box_right);
	}

	inline static bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis) 
	{
		AABB box_a;
		AABB box_b;

		if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
			std::cerr << "No bounding box in bvh_node constructor.\n";

		return box_a.min().e[axis] < box_b.min().e[axis];
	}

	static bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) 
	{
		return box_compare(a, b, 0);
	}

	static bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) 
	{
		return box_compare(a, b, 1);
	}

	static bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) 
	{
		return box_compare(a, b, 2);
	}

	virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& box) const override;

	shared_ptr<Hittable> left;
	shared_ptr<Hittable> right;
	AABB BVH_Box;
};

bool BVH_Node::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const
{
	if (!BVH_Box.hit(ray, t_min, t_max)) return false;

	bool hit_left = left->hit(ray, t_min, t_max, rec);
	bool hit_right = right->hit(ray, t_min, hit_left ? rec.t : t_max, rec);

	return hit_left || hit_right;
}
bool BVH_Node::bounding_box(double time0, double time1, AABB& box) const
{
	box = BVH_Box;
	return true;
}

#endif