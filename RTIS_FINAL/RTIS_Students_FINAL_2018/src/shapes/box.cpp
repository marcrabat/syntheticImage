#include "box.h"

Box::Box(const Vector3D min, const Vector3D max, const Matrix4x4 &t_, Material *material_):
	Shape(t_,material_)
{
	v0 = min;
	v1 = Vector3D(min.x, max.y, min.z);
	v2 = Vector3D(max.x, max.y, min.z);
	v3 = Vector3D(max.x, min.y, min.z);
	v4 = Vector3D(min.x, min.y, max.z);
	v5 = Vector3D(min.x, max.y, max.z);
	v6 = max;
	v7 = Vector3D(max.x, min.y, max.z);

	boundingBox.push_back(new Triangle(v0, v1, v2, material));
	boundingBox.push_back(new Triangle(v0, v3, v2, material));
	boundingBox.push_back(new Triangle(v0, v1, v5, material));
	boundingBox.push_back(new Triangle(v0, v4, v5, material));
	boundingBox.push_back(new Triangle(v0, v4, v7, material));
	boundingBox.push_back(new Triangle(v0, v3, v7, material));

	boundingBox.push_back(new Triangle(v6, v2, v3, material));
	boundingBox.push_back(new Triangle(v6, v7, v3, material));
	boundingBox.push_back(new Triangle(v6, v5, v4, material));
	boundingBox.push_back(new Triangle(v6, v7, v4, material));
	boundingBox.push_back(new Triangle(v6, v5, v1, material));
	boundingBox.push_back(new Triangle(v6, v2, v1, material));
}

bool Box::rayIntersect(const Ray & ray, Intersection & its) const
{
	if (Utils::hasIntersection(ray, boundingBox))
		return true;

	return false;
}

bool Box::rayIntersectP(const Ray & ray) const
{
	if (Utils::hasIntersection(ray, boundingBox))
		return true;

	return false;
}
