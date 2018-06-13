#include "mesh.h"

#include <cassert>
#include <iostream>
#include <limits>
#include <sys/stat.h>

#include "triangle.h"
#include "box.h"
#include "../materials/material.h"
#include "../core/utils.h"


std::map<std::string, Mesh*> Mesh::sMeshesLoaded;

std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings = false);

Mesh::Mesh(const std::string &name, const Matrix4x4 &t_, Material *material_, bool hasBoundingBox_)
	:Shape(t_, material_)
{
	this->hasBoundingBox = hasBoundingBox_;
	loadOBJ(name.c_str());
	if (hasBoundingBox == true) {
		std::cout << "Executing with Bounding Box: \n";
		boundingBox = new Box(header.aabb_min, header.aabb_max, objectToWorld, material);
	}
	else {
		std::cout << "Executing without Bounding Box: \n";
	}
	
}

void Mesh::clear()
{
	//buffers
	vertices.clear();
	normals.clear();
	uvs.clear();
}


bool Mesh::rayIntersect(const Ray & ray, Intersection & its) const
{
	if (this->hasBoundingBox == true) {
		if (!boundingBox->rayIntersectP(ray))
			return false;
	}
	
	if (Utils::getClosestIntersection(ray, triangles, its))
		return true;

	return false;
}

bool Mesh::rayIntersectP(const Ray & ray) const
{
	if (this->hasBoundingBox == true) {
		if (!boundingBox->rayIntersectP(ray))
			return false;
	}
	

	if (Utils::hasIntersection(ray, triangles))
		return true;

	return false;
}

void Mesh::printHeaderInfo()
{
	std::cout << "\n\n Here is the header info of the mesh: \n" << std::endl;
	std::cout << "aabb_max: " << header.aabb_max << "\n" << std::endl;
	std::cout << "aabb_min: " << header.aabb_min << "\n" << std::endl;
	std::cout << "center: " << header.center << "\n" << std::endl;
	std::cout << "halfsize: " << header.halfsize << "\n" << std::endl;
	std::cout << "radius: " << header.radius << "\n" << std::endl;
	std::cout << "size: " << header.size << "\n" << std::endl;
}

bool Mesh::loadOBJ(const char* filename)
{
	struct stat stbuffer;

	FILE* f = fopen(filename, "rb");
	if (f == NULL)
	{
		std::cerr << "File not found: " << filename << std::endl;
		return false;
	}

	stat(filename, &stbuffer);

	unsigned int size = stbuffer.st_size;
	char* data = new char[size + 1];
	fread(data, size, 1, f);
	fclose(f);
	data[size] = 0;

	char* pos = data;
	char line[255];
	int i = 0;

	std::vector<Vector3D> indexed_positions;
	std::vector<Vector3D> indexed_normals;
	std::vector<Vector2D> indexed_uvs;


	Vector3D max(-INFINITY);
	Vector3D min(INFINITY);

	header.aabb_max = max;
	header.aabb_min = min;

	unsigned int vertex_i = 0;

	//parse file
	while (*pos != 0)
	{
		if (*pos == '\n') pos++;
		if (*pos == '\r') pos++;

		//read one line
		i = 0;
		while (i < 255 && pos[i] != '\n' && pos[i] != '\r' && pos[i] != 0) i++;
		memcpy(line, pos, i);
		line[i] = 0;
		pos = pos + i;

		if (*line == '#' || *line == 0) continue; //comment

												  //tokenize line
		std::vector<std::string> tokens = tokenize(line, " ");

		if (tokens.empty()) continue;

		if (tokens[0] == "v" && tokens.size() == 4)
		{
			Vector3D v((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()), (float)atof(tokens[3].c_str()));
			indexed_positions.push_back(v);

			header.aabb_min.setMin(v);
			header.aabb_max.setMax(v);
		}
		else if (tokens[0] == "vt" && tokens.size() == 4)
		{
			Vector2D v((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()));
			indexed_uvs.push_back(v);
		}
		else if (tokens[0] == "vn" && tokens.size() == 4)
		{
			Vector3D v((float)atof(tokens[1].c_str()), (float)atof(tokens[2].c_str()), (float)atof(tokens[3].c_str()));
			indexed_normals.push_back(v);
		}
		else if (tokens[0] == "s") //surface? it appears one time before the faces
		{
			//process mesh
			if (uvs.size() == 0 && indexed_uvs.size())
				uvs.resize(1);
		}
		else if (tokens[0] == "f" && tokens.size() >= 4)
		{
			Vector3D v1, v2, v3;
			v1.parseFromText(tokens[1].c_str(), '/');

			for (unsigned int iPoly = 2; iPoly < tokens.size() - 1; iPoly++)
			{
				v2.parseFromText(tokens[iPoly].c_str(), '/');
				v3.parseFromText(tokens[iPoly + 1].c_str(), '/');

				vertices.push_back(indexed_positions[(unsigned int)(v1.x) - 1]);
				vertices.push_back(indexed_positions[(unsigned int)(v2.x) - 1]);
				vertices.push_back(indexed_positions[(unsigned int)(v3.x) - 1]);

				//Here we create the triangles and add them to the list.
				Vector3D pA = vertices[vertex_i];
				Vector3D pB = vertices[vertex_i+1];
				Vector3D pC = vertices[vertex_i+2];

					
				Triangle * t = new Triangle( pC, pB, pA, material);
				triangles.push_back(t);
	
				vertex_i += 3;

				if (indexed_uvs.size() > 0)
				{
					uvs.push_back(indexed_uvs[(unsigned int)(v1.y) - 1]);
					uvs.push_back(indexed_uvs[(unsigned int)(v2.y) - 1]);
					uvs.push_back(indexed_uvs[(unsigned int)(v3.y) - 1]);
				}

				if (indexed_normals.size() > 0)
				{
					normals.push_back(indexed_normals[(unsigned int)(v1.z) - 1]);
					normals.push_back(indexed_normals[(unsigned int)(v2.z) - 1]);
					normals.push_back(indexed_normals[(unsigned int)(v3.z) - 1]);
				}
			}
		}
	}

	header.center = (header.aabb_max + header.aabb_min) * 0.5;
	header.halfsize = (header.aabb_max - header.center);
	header.radius = (float)fmax(header.aabb_max.length(), header.aabb_min.length());

	return true;
}


std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings)
{
	std::vector<std::string> tokens;

	std::string str;
	size_t del_size = strlen(delimiters);
	const char* pos = source.c_str();
	char in_string = 0;
	unsigned int i = 0;
	while (*pos != 0)
	{
		bool split = false;

		if (!process_strings || (process_strings && in_string == 0))
		{
			for (i = 0; i < del_size && *pos != delimiters[i]; i++);
			if (i != del_size) split = true;
		}

		if (process_strings && (*pos == '\"' || *pos == '\''))
		{
			if (!str.empty() && in_string == 0) //some chars remaining
			{
				tokens.push_back(str);
				str.clear();
			}

			in_string = (in_string != 0 ? 0 : *pos);
			if (in_string == 0)
			{
				str += *pos;
				split = true;
			}
		}

		if (split)
		{
			if (!str.empty())
			{
				tokens.push_back(str);
				str.clear();
			}
		}
		else
			str += *pos;
		pos++;
	}
	if (!str.empty())
		tokens.push_back(str);
	return tokens;
}