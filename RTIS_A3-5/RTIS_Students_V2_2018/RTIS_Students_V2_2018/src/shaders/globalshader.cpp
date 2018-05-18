#include "globalshader.h"
#include "../materials/phong.h"

GlobalShader::GlobalShader(Vector3D color_, double maxDist_, Vector3D bgColor_, Vector3D at)
	:Shader(bgColor_), maxDist(maxDist_), color(color_)
{
	this->at = at;
}

Vector3D GlobalShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const
{
	Vector3D Lo = bgColor;
	Vector3D LoDir = Vector3D(0);
	Vector3D LoInd = Vector3D(0);
	Intersection its;

	if (Utils::getClosestIntersection(r, objList, its)) {
		
		bool hasDiffuseOrGlossy = its.shape->getMaterial().hasDiffuseOrGlossy();
		bool hasTransmission = its.shape->getMaterial().hasTransmission();
		bool hasSpecular = its.shape->getMaterial().hasSpecular();

		Vector3D p = its.itsPoint;
		Vector3D n = its.normal;
		
		if (hasDiffuseOrGlossy) {
			for (int ls = 0; ls < lsList.size(); ls++) {
				
				Vector3D Li = lsList.at(ls).getIntensity(p);
				Vector3D wi = lsList.at(ls).getPosition() - p;
				Vector3D wo = -r.d;
				
				Ray rayShadow = Ray(p, wi.normalized());
				rayShadow.maxT = wi.length();
				
				bool visible =  (dot(n,wi) > 0) ? true : false;

				if (visible) {
					if (!Utils::hasIntersection(rayShadow, objList)) {
						Vector3D r = its.shape->getMaterial().getReflectance(n.normalized(), wo.normalized(), wi.normalized());
						LoDir += Utils::multiplyPerCanal(Li, r);
					}
				}
			}
			//here we have to compute indirect illumination
			if (typeid(its.shape->getMaterial()).name() == typeid(Phong).name()) {
				Vector3D kd = its.shape->getMaterial().getDiffuseCoefficient();
				Vector3D at = this->at;
				LoInd += dot(kd, at);
			}
			Lo += LoDir + LoInd;
		}

		if (hasSpecular) {
			Vector3D wr = Utils::computeReflectionDirection(r.d, n);
			Ray reflectionRay = Ray(p, wr, r.depth + 1);
			Lo = computeColor(reflectionRay, objList, lsList);
		}

		if (hasTransmission) {
			double eta = its.shape->getMaterial().getIndexOfRefraction();
			double cosThetaI = dot(-r.d, n);

			if (cosThetaI < 0){
				n *= -1;
				cosThetaI = dot(-r.d, n);
				eta = 1 / its.shape->getMaterial().getIndexOfRefraction();
			}

			double cosThetaT;
			if (!Utils::isTotalInternalReflection(eta, cosThetaI, cosThetaT)) {
				cosThetaT = sqrt(1 + pow(eta,2) * (pow(cosThetaI,2) - 1));
				Vector3D wt = Utils::computeTransmissionDirection(r, n, eta, cosThetaI, cosThetaT);
				Ray refracRay = Ray(p, wt, r.depth + 1);
				Lo = computeColor(refracRay, objList, lsList);
				
			}
			else {
				Vector3D wr = Utils::computeReflectionDirection(r.d, n);
				Ray reflectionRay = Ray(p, wr, r.depth + 1);
				Lo = computeColor(reflectionRay, objList, lsList);
			}

		}

		return Lo;
	}
	
}


