#include "ortographic.h"

OrtographicCamera::OrtographicCamera(const Matrix4x4 &cameraToWorld_,
                  const Film &film_ )
    : Camera(cameraToWorld_, film_)
{ }


Vector3D OrtographicCamera::ndcToCameraSpace(const double u, const double v) const
{
    // Compute the x-coordinate in camera space (taking into account the aspect ratio)
    double x = ((u * 2) - 1) * aspect;
    // Compute the x-coordinate in camera space
    double y = ((v * 2) - 1);

    // Return the position in camera space
    return Vector3D(x, y, 0);
}


// Input in image space
Ray OrtographicCamera::generateRay(const double u, const double v) const
{
    // Convert the from ndc to camera coordinates
    Vector3D rOrigin = ndcToCameraSpace(u, v); //All rays origins lie on the image plane
	Vector3D rDirection = Vector3D(0, 0, 1); //All rays in ortographic camera have the same direction 

    // COMPLETE THE REST OF THE FUNCTION
	rDirection = rDirection.normalized();
	Ray rayWorldCoords = Ray(rOrigin, rDirection);
	rayWorldCoords = cameraToWorld.transformRay(rayWorldCoords);
    
    return rayWorldCoords;
}
