#include "globalshader.h"
#include "../materials/phong.h"
#include "../sampler/hemisphericalsampler.h"
#include <math.h>

GlobalShader::GlobalShader(Vector3D color_, double maxDist_, Vector3D bgColor_, Vector3D at_)
	:Shader(bgColor_), maxDist(maxDist_), color(color_), at(at_)
{
}

Vector3D GlobalShader::computeColor(const Ray &ray, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const
{
	Intersection its;
	Vector3D Lo = bgColor;

	if (Utils::getClosestIntersection(ray, objList, its))
	{

		Vector3D LoDir = Vector3D(0);
		Vector3D LoInd = Vector3D(0);

		Vector3D p = its.itsPoint;
		Vector3D normal = its.normal;
		Vector3D wo = -ray.d;

		bool hasDiffuseOrGlossy = its.shape->getMaterial().hasDiffuseOrGlossy();
		bool hasSpecular = its.shape->getMaterial().hasSpecular();
		bool hasTransmission = its.shape->getMaterial().hasTransmission();

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


			if (ray.depth == 0)
			{
				int nSamples = 500;
				HemisphericalSampler sampler;

				for (int i = 0; i < nSamples; i++) {
					Vector3D wj = sampler.getSample(normal);
					Ray secondaryRay = Ray(p, wj, ray.depth + 1);

					Vector3D r = its.shape->getMaterial().getReflectance(normal.normalized(), wo.normalized(), wj.normalized());
					LoInd += Utils::multiplyPerCanal(computeColor(secondaryRay, objList, lsList), r);
				}
				LoInd /= 2 * M_PI*nSamples;
			}
			else if (ray.depth == this->maxDist) {
				Vector3D kd = its.shape->getMaterial().getDiffuseCoefficient();
				Vector3D at = this->at;
				LoInd = Utils::multiplyPerCanal(at, kd);
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////
			else {
				Ray n_ray = Ray(p, normal, ray.depth + 1);
				Vector3D r = Utils::computeReflectionDirection(wo, normal);
				Ray perfect_ray = Ray(p, r, ray.depth + 1);

				double aux = dot(-n_ray.d, r);
				aux += dot(-perfect_ray.d, r);

				LoInd = 1 / (4 * M_PI) * aux;

			}
			Lo = LoDir + LoInd;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		if (hasTransmission) {
			double eta = its.shape->getMaterial().getIndexOfRefraction();
			double cosThetaI = dot(wo, normal);

			if (cosThetaI < 0) {
				normal *= -1;
				cosThetaI = dot(wo, normal);
				eta = 1 / its.shape->getMaterial().getIndexOfRefraction();
			}

			double cosThetaT;
			if (!Utils::isTotalInternalReflection(eta, cosThetaI, cosThetaT)) {
				cosThetaT = sqrt(1 + pow(eta, 2) * (pow(cosThetaI, 2) - 1));
				Vector3D wt = Utils::computeTransmissionDirection(ray, normal, eta, cosThetaI, cosThetaT);
				Ray refractionRay = Ray(p, wt, ray.depth);
				Lo = computeColor(refractionRay, objList, lsList);

			}
			else {
				Vector3D wr = Utils::computeReflectionDirection(ray.d, normal);
				Ray reflectionRay = Ray(p, wr, ray.depth);
				Lo = computeColor(reflectionRay, objList, lsList);
			}

		}
		if (hasSpecular) {
			Vector3D wr = Utils::computeReflectionDirection(ray.d, normal);
			Ray reflectionRay = Ray(p, wr, ray.depth);
			Lo = computeColor(reflectionRay, objList, lsList);
		}
	}

	return Lo;
}

