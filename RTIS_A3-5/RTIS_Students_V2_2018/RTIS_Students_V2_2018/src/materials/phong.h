#ifndef PHONG
#define PHONG

#include "material.h"

class Phong : public Material
{
private:
	Vector3D ks,kd;
	float n; //shininess coefficient, not normal!
	
public:
	Phong(Vector3D ks_, Vector3D kd_, float n_);
	bool Phong::hasSpecular() const;
	bool Phong::hasTransmission() const;
	bool Phong::hasDiffuseOrGlossy() const;
	virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const;
	Vector3D computeDiffuseReflection(Vector3D kd, const Vector3D & normal, const Vector3D & wi) const;
	Vector3D computeSpecularReflection(Vector3D ks, const Vector3D & normal, float shininess, const Vector3D & wo, const Vector3D & wi) const;

};


#endif // PHONG
