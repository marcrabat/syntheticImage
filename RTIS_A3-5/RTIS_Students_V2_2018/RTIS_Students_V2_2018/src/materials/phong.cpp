#include "phong.h"

Phong::Phong(Vector3D ks_, Vector3D kd_, float n_): ks(ks_), kd(kd_), n(n_){}

bool Phong::hasSpecular() const
{
    // This material does not have specular component
    return false;
}

bool Phong::hasTransmission() const
{
    return false;
}

bool Phong::hasDiffuseOrGlossy() const
{
    return true;
}

Vector3D Phong::computeDiffuseReflection(Vector3D kd, const Vector3D & normal, const Vector3D & wi) const
{
	return kd * dot(normal, wi);
}

Vector3D Phong::computeSpecularReflection(Vector3D ks, const Vector3D & normal, float shininess, const Vector3D & wo, const Vector3D & wi) const
{
	Vector3D wr = normal * 2 * dot(normal, wi) - wi;
	return ks * pow(dot(wo,wr), shininess);
}

Vector3D Phong::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const
{
	Vector3D reflectance;
	Vector3D diffuse_reflectance, specular_reflectance;
	Vector3D normal = n;
	diffuse_reflectance = computeDiffuseReflection(this->kd, normal, wi);
	specular_reflectance = computeSpecularReflection(this->ks, normal, this->n, wo, wi);
	reflectance = diffuse_reflectance + specular_reflectance;
	return reflectance;
}


