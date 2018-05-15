#include "transmissive.h"



Transmissive::Transmissive()
{
}

Transmissive::Transmissive(const double r, const Vector3D & color)
{
	this->r = r;
	this->color = color;
}


Vector3D Transmissive::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const
{
	return Vector3D();
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