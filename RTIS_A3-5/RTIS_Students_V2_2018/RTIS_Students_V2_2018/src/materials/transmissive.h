#pragma once
#include "material.h"

class Transmissive : public Material
{
public:
	Transmissive();
	Transmissive(const double t , const Vector3D &color);

	Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,const Vector3D &wi) const;
	bool hasSpecular() const;
	bool hasTransmission() const;
	bool hasDiffuseOrGlossy() const;
	double getIndexOfRefraction() const;

private:
	double t;
	Vector3D color;
};

