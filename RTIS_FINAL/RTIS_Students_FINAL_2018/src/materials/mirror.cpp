#include "mirror.h"



Mirror::Mirror()
{
}

Mirror::Mirror(const Vector3D & color_) : color(color_)
{
	//this->color = color;
}

Vector3D Mirror::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const
{
	return color;
}

bool Mirror::hasSpecular() const
{
	return true;
}

bool Mirror::hasTransmission() const
{
	return false;
}

bool Mirror::hasDiffuseOrGlossy() const
{
	return false;
}

double Mirror::getIndexOfRefraction() const
{
	return 0.0;
}
