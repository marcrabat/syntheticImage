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
	Mesh(const std::string& name, const Matrix4x4 &t_, Material *material_, const std::string texture = "");

	bool loadOBJ(const char* filename);
	void clear();

	bool rayIntersect(const Ray &ray, Intersection &its) const;
	bool rayIntersectP(const Ray &ray) const;
	
	static std::map<std::string, Mesh*> sMeshesLoaded;

	std::string name;
	std::vector< Vector3D > vertices; //here we store the vertices
	std::vector< Vector3D > normals;	 //here we store the normals
	std::vector< Vector2D > uvs;	 //here we store the texture coordinates

	std::string texture;

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

	//collision testing
	/*void* collision_model;
	bool createCollisionModel(bool is_static = false); //is_static sets if the inv matrix should be computed after setTransform (true) or before rayCollision (false)											   //help: model is the transform of the mesh, ray origin and direction, a Vector3 where to store the collision if found, a Vector3 where to store the normal if there was a collision, max ray distance in case the ray should go to infintiy, and in_object_space to get the collision point in object space or world space
	bool testRayCollision(Matrix4x4 model, Vector3D ray_origin, Vector3D ray_direction, Vector3D& collision, Vector3D& normal, float max_ray_dist = 3.4e+38F, bool in_object_space = false);
	*/

};

#endif