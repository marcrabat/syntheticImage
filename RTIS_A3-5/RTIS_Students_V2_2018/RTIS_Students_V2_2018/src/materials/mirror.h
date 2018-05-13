#pragma once
#include "material.h"

class Mirror : public Material
{
public:
	Mirror();
	Mirror(const Vector3D &color);

	Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,const Vector3D &wi) const;
	bool hasSpecular() const;
	bool hasTransmission() const;
	bool hasDiffuseOrGlossy() const;
	double getIndexOfRefraction() const;

private:
	Vector3D color;
};

