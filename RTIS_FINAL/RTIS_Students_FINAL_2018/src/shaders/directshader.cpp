#include "directshader.h"

DirectShader::DirectShader(Vector3D bgColor_)
	:Shader(bgColor_)
{}

Vector3D DirectShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const 
{

	Vector3D color;
	Intersection its;

	if (Utils::getClosestIntersection(r, objList, its)) {

		Vector3D p = its.itsPoint;
		
		Vector3D n = its.normal;

		for (int ls = 0; ls < lsList.size(); ls++) {
			const PointLightSource light = lsList.at(ls);

			Vector3D wi = light.getPosition() - p;
			double maxT = wi.length();
			Vector3D wo = -r.d;

			if (dot(n, wi) > 0) {

				Vector3D reflectance;
				reflectance = its.shape->getMaterial().getReflectance(n.normalized(), wo.normalized(), wi.normalized());

				Ray rayShadow = Ray(p + n * 0.0001, wi.normalized());
				rayShadow.maxT = wi.length();

				if (!Utils::hasIntersection(rayShadow, objList)) {
					color += Utils::multiplyPerCanal(light.getIntensity(p), reflectance);		
				}
			}


		}
		return color;
	}
	else {
		return bgColor;
	}
	
}


