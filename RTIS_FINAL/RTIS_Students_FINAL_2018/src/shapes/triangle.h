#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"

class Triangle : public Shape
{
public:
    Triangle() = delete;
	Triangle(const Vector3D &p0_, const Vector3D &normal_,
                 Material *mat_);

    // Get the normal at a surface point in world coordinates
    Vector3D getNormalWorld() const;

    // Ray/plane intersection methods
    virtual bool rayIntersect(const Ray &ray, Intersection &its) const;
    virtual bool rayIntersectP(const Ray &ray) const;

    // Convert triangle to String
    std::string toString() const;

private:
    // A point belonging to the plane, and the normal at the plane
    /* All values are in world coordinates */
    Vector3D p0World;
    Vector3D nWorld;
};

std::ostream &operator<<(std::ostream &out, const Triangle &t);

#endif // INFINITEPLANE_H
