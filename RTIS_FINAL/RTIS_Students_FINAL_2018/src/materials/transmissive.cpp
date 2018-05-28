#include "transmissive.h"



Transmissive::Transmissive()
{
}

Transmissive::Transmissive(double r_, Vector3D & color_):r(r_),color(color_)
{
}


Vector3D Transmissive::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const
{
	return this->color;
}

bool Transmissive::hasSpecular() const
{
	return false;
}

bool Transmissive::hasTransmission() const
{
	return true;
}

bool Transmissive::hasDiffuseOrGlossy() const
{
	return false;
}

double Transmissive::getIndexOfRefraction() const
{
	return this->r;
}