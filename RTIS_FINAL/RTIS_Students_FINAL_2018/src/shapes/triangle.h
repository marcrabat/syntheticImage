#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "../core/vector2d.h"

class Triangle : public Shape
{
public:
    Triangle() = delete;
	Triangle(const Vector3D &pA_, const Vector3D &pB_, const Vector3D &pC_, Material *mat_);

    // Get the normal at a surface point in world coordinates
    Vector3D getNormalWorld() const;

    // Ray/plane intersection methods
    virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
    virtual bool rayIntersectP(const Ray &ray) const;

    // Convert triangle to String
    std::string toString() const;

	Vector3D pA, pB, pC;
    Vector3D n;
};

std::ostream &operator<<(std::ostream &out, const Triangle &t);

#endif // INFINITEPLANE_H
