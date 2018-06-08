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

		if (its.shape->getMaterial().hasTransmission()) {
			double eta = its.shape->getMaterial().getIndexOfRefraction();
			double cosThetaI = dot(-r.d, n);

			if (cosThetaI < 0) {
				n *= -1;
				cosThetaI = dot(-r.d, n);
				eta = 1 / its.shape->getMaterial().getIndexOfRefraction();
			}

			double cosThetaT;
			if (!Utils::isTotalInternalReflection(eta, cosThetaI, cosThetaT)) {
				cosThetaT = sqrt(1 + eta * eta * (cosThetaI * cosThetaI - 1));
				Vector3D wt = Utils::computeTransmissionDirection(r, n, eta, cosThetaI, cosThetaT);
				Ray refracRay = Ray(p, wt, r.depth + 1);
				color = computeColor(refracRay, objList, lsList);

			}
			else {
				Vector3D wr = Utils::computeReflectionDirection(r.d, n);
				Ray reflectionRay = Ray(p, wr, r.depth + 1);
				color = computeColor(reflectionRay, objList, lsList);
			}

		}

		if (its.shape->getMaterial().hasSpecular()) {
			Vector3D wr = Utils::computeReflectionDirection(r.d, n);
			Ray reflectionRay = Ray(p, wr, r.depth + 1);
			color = computeColor(reflectionRay, objList, lsList);
		}

		//if we are working with textures
		//(...)

		for (int ls = 0; ls < lsList.size(); ls++) {
			const PointLightSource light = lsList.at(ls);

			Vector3D wi = light.getPosition() - p;
			double maxT = wi.length();
			Vector3D wo = -r.d;

			if (dot(n, wi) <= 0) {

				Vector3D reflectance;
				reflectance = its.shape->getMaterial().getReflectance(n.normalized(), wo.normalized(), wi.normalized());

				Ray rayShadow = Ray(p, wi.normalized());
				rayShadow.maxT = wi.length();

				//color = Vector3D(1.0, 0.0, 0.0);
				if (!Utils::hasIntersection(rayShadow, objList)) {
					color += Utils::multiplyPerCanal(light.getIntensity(p), reflectance);
					//std::cout << "color: " << color << std::endl;

					//color = Vector3D(0.0, 1.0, 0.0);
					
				}
			}


		}
		return color;
	}
	else {
		return bgColor;
	}
	
}


