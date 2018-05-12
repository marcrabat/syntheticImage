#include "directshader.h"

DirectShader::DirectShader(Vector3D color_, double maxDist_, Vector3D bgColor_)
	:Shader(bgColor_), maxDist(maxDist_), color(color_)
{}

Vector3D DirectShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const 
{

	Vector3D Lo = bgColor;
	Intersection its;

	if (Utils::getClosestIntersection(r, objList, its)) {
		for (int lsIndex = 0; lsIndex < lsList.size(); lsIndex++) {
			const PointLightSource light = lsList.at(lsIndex);

			Vector3D wi = light.getPosition() - its.itsPoint;
			Vector3D wo = -r.d;
			Vector3D n = its.normal;
			Vector3D reflectance = its.shape->getMaterial().getReflectance(n.normalized(), wo.normalized(), wi.normalized());
			Vector3D p = its.itsPoint;

			if (dot(n, wi) > 0) {
				Lo += Utils::multiplyPerCanal(light.getIntensity(p), reflectance);
			}

		}

	}
	return Lo;
}