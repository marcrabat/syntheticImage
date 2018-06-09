#include "triangle.h"

Triangle::Triangle(const Vector3D &pA_, const Vector3D &pB_, const Vector3D &pC_,
	Material *mat_) :
	Shape(Matrix4x4(), mat_),
	pA(pA_), pB(pB_), pC(pC_)
{ 
	this->n = cross((pC_ - pA_), (pB_ - pA_)).normalized();
}

Vector3D Triangle::getNormalWorld() const
{
	return this->n;
}

bool Triangle::rayIntersectP(const Ray &rayWorld) const
{
	float denominator = dot(rayWorld.d, n);

	if (std::abs(denominator) < Epsilon) {
		return false;
	}

	float tHit = dot((pA - rayWorld.o), n) / denominator;
	Vector3D pHit = rayWorld.o + rayWorld.d * tHit;

	if (tHit < rayWorld.minT || tHit > rayWorld.maxT)
		return false;
	
	Vector3D vAB = pB - pA;
	Vector3D vBC = pC - pB;
	Vector3D vCA = pA - pC;

	Vector3D vAP = pHit - pA;
	Vector3D vBP = pHit - pB;
	Vector3D vCP = pHit - pC;
	
	if (dot(cross(vAP, vAB).normalized(), n) > 0) {
		if (dot(cross(vBP, vBC).normalized(), n) > 0) {
			if (dot(cross(vCP, vCA).normalized(), n) > 0)
				return true;
		}
	}
	return false;
}

bool Triangle::rayIntersect(const Ray &rayWorld, Intersection &its) const
{
	float denominator = dot(rayWorld.d, n);

	if (std::abs(denominator) < Epsilon) {
		return false;
	}

	float tHit = dot((pA - rayWorld.o), n) / denominator;

	if (tHit < rayWorld.minT || tHit > rayWorld.maxT)
		return false;

	Vector3D pHit = rayWorld.o + rayWorld.d * tHit;

	Vector3D vAB = pB - pA;
	Vector3D vBC = pC - pB;
	Vector3D vCA = pA - pC;

	Vector3D vAP = pHit - pA;
	Vector3D vBP = pHit - pB;
	Vector3D vCP = pHit - pC;

	if (dot(cross(vAP, vAB), n) > 0) {
		if (dot(cross(vBP, vBC), n) > 0) {
			if (dot(cross(vCP, vCA), n) > 0) {

				its.itsPoint = objectToWorld.transformPoint(pHit);
				its.normal = getNormalWorld();
				its.shape = this;
				rayWorld.maxT = tHit;
				return true;

			}
		}
	}


	return false;
}

std::string Triangle::toString() const
{
	std::stringstream s;
	s << "\tInfinitePlan attributes: \n";

	s << "\t p0 (world coordinates): " << pA << "\n";
	s << "\t normal (world coordinates): " << n << "\n" << std::endl;

	return s.str();
}

std::ostream &operator<<(std::ostream &out, const Triangle &t)
{
	out << t.toString();
	return out;
}