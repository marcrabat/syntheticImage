#include "directshader.h"
#include "../core/utils.h"




DirectShader::DirectShader(Vector3D color_, double maxDist_, Vector3D bgColor_): color(color_), Shader(bgColor_){}

Vector3D DirectShader::computeColor(const Ray & r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
	Vector3D intensity;
	Intersection its;
	
	if (!Utils::getClosestIntersection(r, objList, its)) {
		return this->bgColor;
	}

	Vector3D itsPoint = its.itsPoint;
	Vector3D itsNormal = its.normal;
	
	
	Vector3D wo = (r.o - itsPoint).normalized();
	int visibility = 0;


	for (size_t lightIndex = 0; lightIndex < lsList.size(); lightIndex++) {

		//get the light source
		const PointLightSource light = lsList.at(lightIndex);

		//variables that depends on the light
		Vector3D wi = (light.getPosition() - itsPoint).normalized();
		double maxT = wi.length();
		Vector3D li = light.getIntensity(itsPoint);
		Vector3D reflectance = its.shape->getMaterial().getReflectance(itsNormal, wo, wi);

		//to check if the light source is visible from p
		Ray lightRay = Ray(itsPoint, wi);
		if (!Utils::hasIntersection(lightRay, objList)) {
			visibility = 1;

		}

		intensity = dot(li, reflectance) * visibility;

	}
	
	return intensity;
}
