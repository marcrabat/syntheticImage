#include "depthshader.h"
#include "..\core\utils.h"


DepthShader::DepthShader(Vector3D color_, double maxDist_, Vector3D bgColor_)
{
	this->color = color_;
	this->maxDist = maxDist_;
	this->bgColor = bgColor_;
}

Vector3D DepthShader::computeColor(const Ray & r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{

		Intersection closest;

		if (Utils::getClosestIntersection(r, objList, closest)) {
			Vector3D distance = closest.itsPoint - r.o; 
			double depth = sqrt(pow(distance.x, 2) + pow(distance.y, 2) + pow(distance.z, 2));
			if (depth >= this->maxDist) {
				return this->bgColor;
			}
			else {
				return this->color * (1.0 - depth / this->maxDist);

			}
		}
		return this->bgColor;
}
