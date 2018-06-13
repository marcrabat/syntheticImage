#ifndef MESH_H
#define MESH_H

#include <vector>
#include <map>
#include <string>
#include "shape.h"

#include "../core/vector3d.h"
#include "../core/vector2d.h"
#include "../core/matrix4x4.h"

class Mesh : public Shape
{
public:

	Mesh();
	Mesh(const std::string& name, const Matrix4x4 &t_, Material *material_);

	bool loadOBJ(const char* filename);
	Mesh(const std::string & name, const Matrix4x4 & t_, Material * material_, bool hasBoundingBox_);
	void clear();

	bool rayIntersect(const Ray &ray, Intersection &its) const;
	bool rayIntersectP(const Ray &ray) const;
	
	static std::map<std::string, Mesh*> sMeshesLoaded;

	std::string name;
	std::vector< Vector3D > vertices; //here we store the vertices
	std::vector< Vector3D > normals;	 //here we store the normals
	std::vector< Vector2D > uvs;	 //here we store the texture coordinates

	typedef struct
	{
		int size;
		Vector3D aabb_min;
		Vector3D aabb_max;
		Vector3D center;
		Vector3D halfsize;
		float radius;
	} sMeshInfo;

	sMeshInfo header;

	void printHeaderInfo();

	std::vector<Shape*> triangles;
	Shape* boundingBox;
	bool hasBoundingBox = false;

};

#endif