#include "shader.h"

class DirectShader : public Shader
{
public:
	DirectShader(Vector3D bgColor_);

	Vector3D computeColor(const Ray &r,
		const std::vector<Shape*> &objList,
		const std::vector<PointLightSource> &lsList) const;

private:
	Vector3D bgColor;
};

