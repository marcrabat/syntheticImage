#ifndef GLOBALSHADER_H
#define GLOBALSHADER_H
#include "shader.h"
#include "../core/utils.h"

class GlobalShader : public Shader
{
public:
	GlobalShader(Vector3D color_, double maxDist_, Vector3D bgColor_, Vector3D at);
	virtual Vector3D computeColor(const Ray &r,
		const std::vector<Shape*> &objList,
		const std::vector<PointLightSource> &lsList) const;

private:
	Vector3D color;
	double maxDist;
	Vector3D at;
};

#endif // DIRECTSHADER_H