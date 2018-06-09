#include "phong.h"
#include "../core/utils.h"

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

Vector3D Phong::computeSpecularReflection(Vector3D ks, const Vector3D & normal, float shininess, const Vector3D & wo, const Vector3D & wi, const Vector3D & wr) const
{
	return ks * pow(dot(wo,wr), shininess);
}

Vector3D Phong::getDiffuseCoefficient() const
{
	return this->kd;
}

Vector3D Phong::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const
{
	Vector3D reflectance;
	Vector3D diffuse, specular;
	Vector3D normal = n;

	Vector3D wr = Utils::computeReflectionDirection(-wi, n);

	/*if (dot(wr, n) > 1.f)
		return Vector3D(0.f);

	if (dot(wi,n) <= 0.0)
		return Vector3D();*/
	
	diffuse = computeDiffuseReflection(this->kd, normal, wi);
	specular = computeSpecularReflection(this->ks, normal, this->n, wo, wi, wr);
	reflectance = diffuse + specular;

	//std::cout << reflectance << std::endl;
	return reflectance;
}


