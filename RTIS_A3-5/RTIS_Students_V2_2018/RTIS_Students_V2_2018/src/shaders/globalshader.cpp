#include "globalshader.h"
#include "../materials/phong.h"
#include "../sampler/hemisphericalsampler.h"
#include <math.h>

GlobalShader::GlobalShader(Vector3D color_, double maxDist_, Vector3D bgColor_, Vector3D at)
	:Shader(bgColor_), maxDist(maxDist_), color(color_)
{
	this->at = at;
}

Vector3D GlobalShader::computeColor(const Ray &ray, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const
{
	Vector3D Lo = bgColor;
	Vector3D LoDir = Vector3D(0);
	Vector3D LoInd = Vector3D(0);
	HemisphericalSampler sampler;
	Intersection its;

	if (Utils::getClosestIntersection(ray, objList, its)) {

		bool hasDiffuseOrGlossy = its.shape->getMaterial().hasDiffuseOrGlossy();
		bool hasTransmission = its.shape->getMaterial().hasTransmission();
		bool hasSpecular = its.shape->getMaterial().hasSpecular();

		Vector3D p = its.itsPoint;
		Vector3D normal = its.normal;
		Vector3D wo = -ray.d;


		if (hasDiffuseOrGlossy) {
			for (int ls = 0; ls < lsList.size(); ls++) {

				Vector3D Li = lsList.at(ls).getIntensity(p);
				Vector3D wi = lsList.at(ls).getPosition() - p;

				Ray rayShadow = Ray(p, wi.normalized());
				rayShadow.maxT = wi.length();

				bool visible = (dot(normal, wi) > 0) ? true : false;

				if (visible) {
					if (!Utils::hasIntersection(rayShadow, objList)) {
						Vector3D r = its.shape->getMaterial().getReflectance(normal.normalized(), wo.normalized(), wi.normalized());
						LoDir += Utils::multiplyPerCanal(Li, r);
					}	
				}
			}
			//TODO: Fer que ens funcioni aixo
			//Part 2.2, funciona pero no se si hauria d'anar aixi
			//comentar if part 2.2 i descomentar 3.3 per provar
			if (typeid(its.shape->getMaterial()).name() == typeid(Phong).name()) {
				Vector3D kd = its.shape->getMaterial().getDiffuseCoefficient();
				LoInd += dot(kd, this->at);
			}
			// fi part 2.2

			// part 3.3
			/*if (ray.depth == 0) {
				if (typeid(its.shape->getMaterial()).name() == typeid(Phong).name()) {
					Vector3D kd = its.shape->getMaterial().getDiffuseCoefficient();
					Lo += dot(kd, this->at);
				}

				int nSamples = 2;
				for (int i = 0; i < nSamples; i++) {
					Vector3D wi = sampler.getSample(normal);
					Vector3D r = its.shape->getMaterial().getReflectance(normal, wo, wi);
					Ray secondaryRay = Ray(p, wi, ray.depth + 1);
					LoInd += Utils::multiplyPerCanal(computeColor(secondaryRay, objList, lsList), r);
				}
				LoInd /= 2 * M_PI*nSamples;
			}
			else if (ray.depth > 0) {
				Vector3D kd = its.shape->getMaterial().getDiffuseCoefficient();
				Vector3D at = this->at;

				LoInd += dot(kd, at);
			}*/

			Lo = LoDir + LoInd;
		}

		if (hasSpecular) {
			Vector3D wr = Utils::computeReflectionDirection(ray.d, normal);
			Ray reflectionRay = Ray(p, wr, ray.depth);
			Lo = computeColor(reflectionRay, objList, lsList);
		}

		if (hasTransmission) {
			double eta = its.shape->getMaterial().getIndexOfRefraction();
			double cosThetaI = dot(-ray.d, normal);

			if (cosThetaI < 0) {
				normal *= -1;
				cosThetaI = dot(-ray.d, normal);
				eta = 1 / its.shape->getMaterial().getIndexOfRefraction();
			}

			double cosThetaT;
			if (!Utils::isTotalInternalReflection(eta, cosThetaI, cosThetaT)) {
				cosThetaT = sqrt(1 + pow(eta, 2) * (pow(cosThetaI, 2) - 1));
				Vector3D wt = Utils::computeTransmissionDirection(ray, normal, eta, cosThetaI, cosThetaT);
				Ray refracRay = Ray(p, wt, ray.depth);
				Lo = computeColor(refracRay, objList, lsList);

			}
			else {
				Vector3D wr = Utils::computeReflectionDirection(ray.d, normal);
				Ray reflectionRay = Ray(p, wr, ray.depth);
				Lo = computeColor(reflectionRay, objList, lsList);
			}

		}

		return Lo;
	}

}


