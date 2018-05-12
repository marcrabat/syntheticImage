#include "directshader.h"

DirectShader::DirectShader(Vector3D color_, double maxDist_, Vector3D bgColor_)
	:Shader(bgColor_), maxDist(maxDist_), color(color_)
{}

Vector3D DirectShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const 
{

	Intersection its;
	Vector3D Lo = bgColor;
	

	if (Utils::getClosestIntersection(r, objList, its)) {
		
		Vector3D p = its.itsPoint;
		Vector3D n = its.normal;

		for (int ls = 0; ls < lsList.size(); ls++) {
			const PointLightSource light = lsList.at(ls);
			
			Vector3D wi = light.getPosition() - its.itsPoint;
			double maxT = wi.length();
			Vector3D wo = -r.d;
			Ray rayShadow = Ray(p, wi.normalized());
			rayShadow.maxT = wi.length();
	
			if (dot(n, wi) > 0) {
				if (!Utils::hasIntersection(rayShadow, objList)) {
					Vector3D reflectance = its.shape->getMaterial().getReflectance(n.normalized(), wo.normalized(), wi.normalized());
					Lo += Utils::multiplyPerCanal(light.getIntensity(p), reflectance);
				}
				
			}

		}

	}
	return Lo;
}