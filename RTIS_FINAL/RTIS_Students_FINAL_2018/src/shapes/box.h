#ifndef BOX_H
#define BOX_H

#include "shape.h"
#include <vector>
#include "triangle.h"
#include "../core/utils.h"


class Box : public Shape
{
public:
	Box() = delete;
	Box(const Vector3D min, const Vector3D max, const Matrix4x4 &t_, Material *material_);

	virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
	virtual bool rayIntersectP(const Ray &ray) const;

	std::vector<Shape*> boundingBox;
	Vector3D v0, v1, v2, v3, v4, v5, v6, v7;

};

#endif

