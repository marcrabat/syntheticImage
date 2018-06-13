#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"

#include "shapes/sphere.h"
#include "shapes/infiniteplane.h"
#include "shapes/mesh.h"

#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#include "shaders/intersectionshader.h"
#include "shaders/depthshader.h"
#include "shaders/directshader.h"
#include "shaders/globalshader.h"

#include "materials/material.h"
#include "materials/phong.h"
#include "materials/mirror.h"
#include "materials/transmissive.h"
#include <math.h>

#include <ctime>


void buildSceneCornellBox(Camera* &cam, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
	/* **************************** */
	/* Declare and place the camera */
	/* **************************** */
	// By default, this gives an ID transform
	//  which means that the camera is located at (0, 0, 0)
	//  and looking at the "+z" direction
	Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(0, 0, -3));
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

	/* ************************** */
	/* DEFINE YOUR MATERIALS HERE */
	/* ************************** */

	Material *redDiffuse = new Phong(Vector3D(0, 0, 0), Vector3D(0.7, 0.2, 0.3), 100);
	Material *greenDiffuse = new Phong(Vector3D(0, 0, 0), Vector3D(0.2, 0.7, 0.3), 100);
	Material *greyDiffuse = new Phong(Vector3D(0, 0, 0), Vector3D(0.8, 0.8, 0.8), 100);
	Material *blueDiffuse = new Phong(Vector3D(0, 0, 0), Vector3D(0.3, 0.2, 0.7), 100);
	Material *transmissive = new Transmissive(1.1, Vector3D(1));
	Material *mirror = new Mirror(Vector3D(1, 0.9, 0.85));
	Material *red_100 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.7, 0.2, 0.3), 100);


	/* ******* */
	/* Objects */
	/* ******* */
	// Create a heterogeneous list of objects of type shape
	// (some might be triangles, other spheres, plans, etc)
	objectsList = new std::vector<Shape*>;
	double offset = 3.0;
	// Define and place a sphere
	Matrix4x4 sphereTransform1;
	double radius = 1;
	sphereTransform1 = Matrix4x4::translate(Vector3D(-offset + radius, -offset + radius, 3.5));
	Shape *s1 = new Sphere(1.5, sphereTransform1, mirror);

	// Define and place a sphere
	Matrix4x4 sphereTransform2;
	sphereTransform2 = Matrix4x4::translate(Vector3D(1.0, 0.0, 2));
	Shape *s2 = new Sphere(1, sphereTransform2, transmissive);

	// Define and place a sphere
	Matrix4x4 sphereTransform3;
	radius = 1;
	sphereTransform3 = Matrix4x4::translate(Vector3D(0.3, -offset + radius, 5));
	Shape *s3 = new Sphere(radius, sphereTransform3, red_100);

	// Store the objects in the object list
	objectsList->push_back(s1);
	objectsList->push_back(s2);
	objectsList->push_back(s3);

	//Define and place infinite plane
	Matrix4x4 idTransform;
	// Construct the Cornell Box
	Shape *leftPlan = new InfinitePlane(Vector3D(-offset, 0, 0), Vector3D(1, 0, 0), redDiffuse);
	Shape *rightPlan = new InfinitePlane(Vector3D(offset, 0, 0), Vector3D(-1, 0, 0), greenDiffuse);
	Shape *topPlan = new InfinitePlane(Vector3D(0, offset, 0), Vector3D(0, -1, 0), greyDiffuse);
	Shape *bottomPlan = new InfinitePlane(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), greyDiffuse);
	Shape *backPlan = new InfinitePlane(Vector3D(0, 0, 3 * offset), Vector3D(0, 0, -1), blueDiffuse);
	objectsList->push_back(leftPlan);
	objectsList->push_back(rightPlan);
	objectsList->push_back(topPlan);
	objectsList->push_back(bottomPlan);
	objectsList->push_back(backPlan);


	/* ****** */
	/* Lights */
	/* ****** */
	//
	// ADD YOUR LIGHT SOURCES HERE
	Vector3D lightPosition1 = Vector3D(0, offset - 1, 2 * offset);
	Vector3D lightPosition2 = Vector3D(0, offset - 1, 0);
	Vector3D lightPosition3 = Vector3D(0, offset - 1, offset);

	Vector3D intensity = Vector3D(7,7,7); // Radiant intensity (watts/sr)
	PointLightSource pointLS1(lightPosition1, intensity);
	PointLightSource pointLS2(lightPosition2, intensity);
	PointLightSource pointLS3(lightPosition3, intensity);

	// DO NOT FORGET TO STORE THE LIGHT SOURCES IN THE "lightSourceList"
	lightSourceList = new std::vector<PointLightSource>;
	lightSourceList->push_back(pointLS1);
	lightSourceList->push_back(pointLS2);
	lightSourceList->push_back(pointLS3);
}



void buildSceneSphere(Camera* &cam, Film* &film,
	std::vector<Shape*>* &objectsList,
	std::vector<PointLightSource>* &lightSourceList)
{
	/* **************************** */
	/* Declare and place the camera */
	/* **************************** */
	// By default, this gives an ID transform
	//  which means that the camera is located at (0, 0, 0)
	//  and looking at the "+z" direction
	Matrix4x4 cameraToWorld;
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

	/* ************************** */
	/* DEFINE YOUR MATERIALS HERE */
	/* ************************** */
	// (...)
	//  EXAMPLE:  Material *green_50 = new Phong (Vector3D(0.2, 0.7, 0.3), Vector3D(0.2, 0.6, 0.2), 50);

	Material *green_50 = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0.2, 0.6, 0.2), 50);
	Material *red_50 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.6, 0.2, 0.2), 50);
	Material *blue_50 = new Phong(Vector3D(0.3, 0.2, 0.7), Vector3D(0.2, 0.2, 0.6), 50);
	Material *grey_50 = new Phong(Vector3D(0.5, 0.5, 0.5), Vector3D(0.5, 0.5, 0.5), 50);

	/* ******* */
	/* Objects */
	/* ******* */
	// Create a heterogeneous list of objects of type shape
	// (some might be triangles, other spheres, plans, etc)
	objectsList = new std::vector<Shape*>;

	// Define and place a sphere
	Matrix4x4 sphereTransform1;
	sphereTransform1 = sphereTransform1.translate(Vector3D(-1.0, -0.5, 2 * std::sqrt(2.0)));
	Shape *s1 = new Sphere(0.25, sphereTransform1, blue_50);

	// Define and place a sphere
	Matrix4x4 sphereTransform2;
	sphereTransform2 = sphereTransform2.translate(Vector3D(1.0, 0.0, 6));
	Shape *s2 = new Sphere(1, sphereTransform2, green_50);

	// Define and place a sphere
	Matrix4x4 sphereTransform3;
	sphereTransform3 = sphereTransform3.translate(Vector3D(0.3, -0.75, 3.5));
	Shape *s3 = new Sphere(0.25, sphereTransform3, red_50);

	//Difinimos un plano floor
	Shape *floor = new InfinitePlane(Vector3D(0, -1, 0), Vector3D(0, 1, 0), grey_50);


	// Store the objects in the object list
	objectsList->push_back(s1);
	objectsList->push_back(s2);
	objectsList->push_back(s3);
	objectsList->push_back(floor);


	/* ****** */
	/* Lights */
	/* ****** */
	//
	// ADD YOUR LIGHT SOURCES HERE
	PointLightSource light1(Vector3D(4, 3, -5), Vector3D(50, 50, 50));
	PointLightSource light2(Vector3D(2, 1, -3), Vector3D(50, 50, 50));
	//PointLightSource light3(Vector3D(0, 0, 10), Vector3D(80, 80, 80));

	// DO NOT FORGET TO STORE THE LIGHT SOURCES IN THE "lightSourceList"
	lightSourceList = new std::vector<PointLightSource>;
	lightSourceList->push_back(light1);
	lightSourceList->push_back(light2);
	//lightSourceList->push_back(light3);

}

void buildFinalProjectScene(Camera* &cam, Film* &film, std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList) 
{
	//materials
	Material *white = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0.6, 0.6, 0.6), 100);

	//scene
	objectsList = new std::vector<Shape*>;

	int whatMesh = 1;
	
	Matrix4x4 mTransform;
	Matrix4x4 cameraToWorld;
	Mesh *m;
	PointLightSource *light1, *light2;
	std::cout << "Select a mesh: \n" << "1.- Deer (40-50sec) \n2.- Cat (20-30sec)\n3.- Lee (3-4min)\n4.- Woman (5-6min)" << std::endl;
	std::cin >> whatMesh;

	bool hasBoundingBox = true;
	char boundingBox = NULL;
	std::cout << "\nDo you want to execute it alongside a Bounding Box?\n";
	std::cout << "Options: Yes/No [y/n], Default Option: Yes\n";
	std::cin >> boundingBox;
	std::cout << "\n";

	if (boundingBox == 'n') {
		hasBoundingBox = false;
	}

	switch (whatMesh) {
	case 1: m = new Mesh("data/obj/deer.obj", mTransform, white, hasBoundingBox);
		cameraToWorld = Matrix4x4::translate(Vector3D(0, 750, -1500));
		light1 = new PointLightSource(Vector3D(0, 900, -1850), Vector3D(2500000, 2500000, 2500000));
		light2 = new PointLightSource(Vector3D(-300, 1200, -1700), Vector3D(2500000, 2500000, 2500000));
		break;
	case 2: m = new Mesh("data/obj/cat.obj", mTransform, white, hasBoundingBox);
		cameraToWorld = Matrix4x4::translate(Vector3D(0, 300, -800));
		light1 = new PointLightSource(Vector3D(50, 400, -550), Vector3D(200000, 200000, 200000));
		light2 = new PointLightSource(Vector3D(-50, 600, -400), Vector3D(200000, 200000, 200000));
		break;
	case 3: m = new Mesh("data/obj/lee.obj", mTransform, white, hasBoundingBox);
		cameraToWorld = Matrix4x4::translate(Vector3D(0, 15, -40));
		light1 = new PointLightSource(Vector3D(4, 3, -30), Vector3D(1000, 1000, 1000));
		light2 = new PointLightSource(Vector3D(2, 1, -30), Vector3D(1000, 1000, 1000));
		break;
	case 4: m = new Mesh("data/obj/woman.obj", mTransform, white, hasBoundingBox);
		cameraToWorld = Matrix4x4::translate(Vector3D(0, 50, -200));
		light1 = new PointLightSource(Vector3D(50, 600, -250), Vector3D(25000, 25000, 25000));
		light2 = new PointLightSource(Vector3D(25, 100, -140), Vector3D(25000, 25000, 25000));
		break;
	default:m = new Mesh("data/obj/deer.obj", mTransform, white, true);
		cameraToWorld = Matrix4x4::translate(Vector3D(0, 750, -1500));
		light1 = new PointLightSource(Vector3D(0, 900, -1950), Vector3D(2500000, 2500000, 2500000));
		light2 = new PointLightSource(Vector3D(-300, 300, -2000), Vector3D(2500000, 2500000, 2500000));
	}
	//camera
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

	//plane

	Material *greyDiffuse = new Phong(Vector3D(0, 0, 0), Vector3D(0.8, 0.8, 0.8), 100);
	Shape *bottomPlan = new InfinitePlane(Vector3D(0, -0.3, 0), Vector3D(0, 1, 0), greyDiffuse);
	objectsList->push_back(bottomPlan);

	objectsList->push_back(m);

	//lights
	lightSourceList = new std::vector<PointLightSource>;
	lightSourceList->push_back(*light1);
	lightSourceList->push_back(*light2);

	m->printHeaderInfo();
}

void raytrace(Camera* &cam, Shader* &shader, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
	unsigned int sizeBar = 40;

	size_t resX = film->getWidth();
	size_t resY = film->getHeight();

	// Main raytracing loop
	// Out-most loop invariant: we have rendered lin lines
	for (size_t lin = 0; lin<resY; lin++)
	{
		// Show progression
		if (lin % (resY / sizeBar) == 0)
			std::cout << ".";

		// Inner loop invariant: we have rendered col columns
		for (size_t col = 0; col<resX; col++)
		{
			// Compute the pixel position in NDC
			double x = (double)(col + 0.5) / resX;
			double y = (double)(lin + 0.5) / resY;

			// Generate the camera ray
			Ray cameraRay = cam->generateRay(x, y);

			// Compute ray color according to the used shader
			Vector3D pixelColor = shader->computeColor(cameraRay, *objectsList, *lightSourceList);

			// Store the pixel color
			film->setPixelValue(col, lin, pixelColor);
		}
	}
}

int main()
{

	//time variables
	unsigned t0, t1;
	t0 = clock();
	std::string separator = "\n----------------------------------------------\n";
	std::string separatorStar = "\n**********************************************\n";
	std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

	// Create an empty film
	Film *film;
	film = new Film(720, 576);




	// Declare the shader
	Vector3D bgColor(0.0, 0.0, 0.0); // Background color (for rays which do not intersect anything)
	Vector3D intersectionColor(1, 0, 0);
	Shader *shader = new IntersectionShader(intersectionColor, bgColor);
	Shader *depthShader = new DepthShader(Vector3D(0.4, 1, 0.4), 2000, bgColor);
	Shader *directShader = new DirectShader(bgColor);
	Shader *globalShader = new GlobalShader(Vector3D(0.4, 1, 0.4), 8, bgColor, Vector3D(0.1, 0.1, 0.1));


	// Declare pointers to all the variables which describe the scene
	Camera *cam;
	std::vector<Shape*> *objectsList;
	std::vector<PointLightSource> *lightSourceList;

	// Build the scene
	buildFinalProjectScene(cam, film, objectsList, lightSourceList);

	// Launch some rays!
	raytrace(cam, directShader, film, objectsList, lightSourceList);

	// Save the final result to file
	std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
	film->save();

	std::cout << "\n\n" << std::endl;

	//execution time
	t1 = clock();
	double time = (double(t1 - t0) / CLOCKS_PER_SEC);
	std::cout << "Execution Time: " << time << std::endl;
	return 0;
}
