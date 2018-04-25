#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"
#include "shapes/sphere.h"
#include "cameras/ortographic.h"
#include "cameras/perspective.h"

void transformationsExercise()
{
	/* *************** */
	/* TRANSFORMATIONS */
	/* *************** */
	std::string separator = "\n----------------------------------------------\n";
	std::string separatorStar = "\n**********************************************\n";

	std::cout << separatorStar << "Construct and visualize transforms" << separatorStar << std::endl;

	// Identity Matrix
	Matrix4x4 idMat; // By default, a matrix4x4 is initialized with the identity matrix
	std::cout << "The content of matrix idMat is: \n" << std::endl;
	std::cout << idMat << separator << std::endl;

	// Translation Matrix
	double deltaX, deltaY, deltaZ;
	deltaX = 1; deltaY = -2; deltaZ = 3;
	Vector3D delta(deltaX, deltaY, deltaZ);
	Matrix4x4 translationMat = Matrix4x4::translate(delta);
	std::cout << "The content of matrix translationMat is: \n" << std::endl;
	std::cout << translationMat << separator << std::endl;

	// Scale Matrix
	//(...)
	double sx, sy, sz;
	sx = 2; sy = 1; sz = -1;
	Vector3D vec2scale(sx, sy, sz);
	Matrix4x4 scaleMatrix = Matrix4x4::scale(vec2scale);
	std::cout << "The content of matrix scaleMatrix is: \n" << std::endl;
	std::cout << scaleMatrix << separator << std::endl;

	// Rotate around X Matrix
	double angleInDegrees = 60;
	double thetaInRadians = Utils::degreesToRadians(angleInDegrees);
	Matrix4x4 rotateXMatrix = Matrix4x4::rotate(thetaInRadians, Vector3D(1, 0, 0));
	std::cout << "The content of matrix rotateXMatrix is: \n" << std::endl;
	std::cout << rotateXMatrix << separator << std::endl;

	// Rotate around an arbitrary axis Matrix
	angleInDegrees = 30;
	thetaInRadians = Utils::degreesToRadians(angleInDegrees);
	Matrix4x4 rotateArtbitraryAxisMatrix = Matrix4x4::rotate(thetaInRadians, Vector3D(1, 1, 1));
	std::cout << "The content of matrix rotateArtitraryAxisMatrix is: \n" << std::endl;
	std::cout << rotateArtbitraryAxisMatrix << separator << std::endl;

	// Transposed and Inversion
	std::cout << separatorStar << "Inverting and Transposing a Matrix" << separatorStar << std::endl;
	Matrix4x4 translationMatTransposed;
	translationMat.transpose(translationMatTransposed);
	std::cout << "The transpose of matrix \n\n" << translationMat << "\n is \n\n"
		<< translationMatTransposed << separator << std::endl;

	Matrix4x4 inverseTranslationMat;
	translationMat.inverse(inverseTranslationMat);
	std::cout << "The inverse of matrix \n\n" << translationMat << "\n is \n\n" << inverseTranslationMat << std::endl;
	std::cout << "And their multiplication should thus give the identity matrix:\n\n";
	std::cout << translationMat.operator*(inverseTranslationMat) << std::endl;

	// Combine here some transforms, and visualize the result
	std::cout << separatorStar << "Combine transforms and visualize the result" << separatorStar << std::endl;

	Matrix4x4 scaleTranslate = (scaleMatrix.operator*(translationMat));
	std::cout << "The content of matrix scaleTranslate is: \n" << std::endl;
	std::cout << scaleTranslate << separator << std::endl;

	Matrix4x4 translateScale = (translationMat.operator*(scaleMatrix));
	std::cout << "The content of matrix translateScale is: \n" << std::endl;
	std::cout << translateScale << separator << std::endl;
}

void normalTransformExercise()
{
	std::string separator = "\n----------------------------------------------\n";

	// NOTE: points, vectors and normals are represented, in this framework, using
	//  the Vector3D class. It is up to the programmer to take care of whether the
	//  Vector3D is representing a point, a vector or a normal. For example, when
	//  applying a transform using a Matrix4x4, the programmer must choose which
	//  "applyTransform" method of the namespace Matrix4x4 should be used.

	Vector3D v(1, 1, 0);
	Vector3D n(-1.0 / sqrt(2), 1.0 / sqrt(2), 0);
	Matrix4x4 S = Matrix4x4::scale(Vector3D(2, 1, 1));

	std::cout << "Vector v = " << v << "\n" << std::endl;
	std::cout << "Normal n = " << n << "\n" << std::endl;
	std::cout << "Scale Matrix: \n" << std::endl;
	std::cout << S << std::endl;

	Vector3D vTransformed = S.transformVector(v);
	std::cout << "Vector v\' = " << vTransformed << "\n" << std::endl;

	Vector3D nTransformed = S.transformVector(n);
	std::cout << "Vector n\' = " << nTransformed << "\n" << std::endl;

	double dotnv = dot(nTransformed, vTransformed);
	std::cout << "Dot(n\', v\') = " << dotnv << "\n" << std::endl;

	S.transpose(S);
	S.inverse(S);

	nTransformed = S.transformVector(n);
	std::cout << "Vector n\' = " << nTransformed << "\n" << std::endl;

	dotnv = dot(nTransformed, vTransformed);
	std::cout << "Dot(n\', v\') = " << dotnv << "\n" << std::endl;





}

void paintingAnImageExercise()
{
	// Define the film (i.e., image) resolution
	size_t resX, resY;
	resX = 512;
	resY = 512;
	Film film(resX, resY);

	double x, y;
	std::vector<double> pndc;

	for (unsigned int col = 0; col < resX; col++)
	{
		for (unsigned int row = 0; row < resY; row++)
		{
			x = ((col + 0.5) / resX); y = ((row + 0.5) / resY);
			pndc.push_back(x); pndc.push_back(y);
			Vector3D color(pndc[0], pndc[1], 0);
			film.setPixelValue(col, row, color);
			pndc.clear();

		}
	}

	// Save the final result to file
	film.save();
}

void filteringAnImageExercise()
{
	// Create two instances of the film class with the same resolution
	int resX, resY;
	resX = 512;
	resY = 512;
	Film f1(resX, resY);
	Film f2(resX, resY);

	// Create the original image
	//  Draw a circle centered at centerX, centerY (in pixels, image space)
	//   and with ray r (also in pixels)
	int centerX = resX / 2;
	int centerY = resY / 2;
	int r = std::min(centerX, centerY) / 2;
	for (int lin = 0; lin<resX; lin++)
	{
		for (int col = 0; col<resY; col++)
		{
			// Use the equation of the sphere to determine the pixel color
			if ((lin - centerX)*(lin - centerX) + (col - centerY)*(col - centerY) < r*r)
				f1.setPixelValue(col, lin, Vector3D(1, 1, 0));
		}
	}

	//f1.save();
	// Filter-related variables
	// Declare here your filter-related variables
	// (e.g., FILTER SIZE)

	Film *film1, *film2, *aux;
	film1 = &f1;
	film2 = &f2;

	int filterSize = 9;
	int val = (filterSize - 1) / 2;
	double red, g, b;
	// Implement here your image filtering algorithm
	for (int ite = 0; ite < 100; ite++) {
		for (int lin = val; lin<resX - val; lin++)
		{
			for (int col = val; col + val <resY - val; col++)
			{
				//Vector3D c(0, 0, 0);
				red = 0;
				g = 0;
				b = 0;

				for (int i = -val; i <= val; i++)
				{
					for (int j = -val; j <= val; j++) {
						red += film1->getPixelValue(i + lin, j + col).x;
						g += film1->getPixelValue(i + lin, j + col).y;
						b += film1->getPixelValue(i + lin, j + col).z;
					}
				}

				int fs = filterSize * filterSize;
				Vector3D color(red / fs, g / fs, b / fs);
				film2->setPixelValue(col, lin, color);
			}
		}
		aux = film1;
		film1 = film2;
		film2 = aux;
	}


	// DO NOT FORGET TO SAVE YOUR IMAGE!
	film1->save();
}


void completeSphereClassExercise()
{
	// Make your intersection tests here
	// (....)
	// Translation Matrix
	int radius = 1;
	Vector3D delta(0, 0, 3);
	Matrix4x4 objectToWorld = Matrix4x4::translate(delta);

	Sphere sphere = Sphere(radius, objectToWorld);

	std::cout << sphere << std::endl;

	Vector3D origin = Vector3D(0, 0, 0);
	Vector3D dir1 = Vector3D(0, 0, 1);
	Vector3D dir2 = Vector3D(0, 1, 0);
	Ray ray1 = Ray(origin, dir1);
	Ray ray2 = Ray(origin, dir2);
	
	if (sphere.rayIntersectP(ray1))
		std::cout << "SPHERE 1 INTERSECT" << std::endl;
	else 
		std::cout << "SPHERE 1 NO INTERSECT" << std::endl;

	if (sphere.rayIntersectP(ray2))
		std::cout << "SPHERE 2 INTERSECT" << std::endl;
	else
		std::cout << "SPHERE 2 NO INTERSECT" << std::endl;
}

void eqSolverExercise()
{
	EqSolver solver;
	rootValues roots;

	double A, B, C;

	A = 5;
	B = 6;
	C = 1;

	bool hasRoots = solver.rootQuadEq(A, B, C, roots);

	if (!hasRoots)
	{
		std::cout << "Equation has no real roots!" << std::endl;
	}
	else
	{
		// SHOW THE SOLUTIONS OF THE EQUATION
		// (...)
		std::cout << roots.values[0] << " , " << roots.values[1] << std::endl;
	}
}

void raytrace()
{
	// Define the film (i.e., image) resolution
	size_t resX, resY;
	resX = 512;
	resY = 512;
	Film film(resX, resY);

	/* ******************* */
	/* Orthographic Camera */
	/* ******************* */
	Matrix4x4 cameraToWorld; // By default, this gives an ID transform
							 // meaning that the camera space = world space
	OrtographicCamera camOrtho(cameraToWorld, film);

	/* ******************* */
	/* Perspective Camera */
	/* ******************* */
	double fovRadians = Utils::degreesToRadians(60);
	PerspectiveCamera camPersp(cameraToWorld, fovRadians, film);

	// Save the final result to file
	film.save();
}

int main()
{
	std::string separator = "\n----------------------------------------------\n";

	std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

	// ASSIGNMENT 1
	//transformationsExercise();
	//normalTransformExercise();
	//paintingAnImageExercise();
	//filteringAnImageExercise();

	// ASSIGNMENT 2
	//eqSolverExercise();
	completeSphereClassExercise();
	//raytrace();

	std::cout << "\n\n" << std::endl;
	return 0;
}