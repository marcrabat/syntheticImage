#ifndef DIRECTSHADER_H
#define DIRECTSHADER_H
#include "shader.h"
#include "../core/utils.h"

class DirectShader : public Shader
{
public:
	DirectShader(Vector3D color_, double maxDist_, Vector3D bgColor_);
	virtual Vector3D computeColor(const Ray &r,
		const std::vector<Shape*> &objList,
		const std::vector<PointLightSource> &lsList) const;

private:
	Vector3D color;
	double maxDist;
};

#endif // DIRECTSHADER_H