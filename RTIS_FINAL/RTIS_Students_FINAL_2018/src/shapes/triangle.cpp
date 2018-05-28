#include "triangle.h"

Triangle::Triangle(const Vector3D &p0_, const Vector3D &normal_,
	Material *mat_) :
	Shape(Matrix4x4(), mat_),
	p0World(p0_), nWorld(normal_.normalized())
{ }

Vector3D Triangle::getNormalWorld() const
{
	return nWorld;
}

bool Triangle::rayIntersectP(const Ray &rayWorld) const
{
	return true;
}

bool Triangle::rayIntersect(const Ray &rayWorld, Intersection &its) const
{
	return true;
}

std::string Triangle::toString() const
{
	std::stringstream s;
	s << "\tInfinitePlan attributes: \n";

	s << "\t p0 (world coordinates): " << p0World << "\n";
	s << "\t normal (world coordinates): " << nWorld << "\n" << std::endl;

	return s.str();
}

std::ostream &operator<<(std::ostream &out, const Triangle &t)
{
	out << t.toString();
	return out;
}