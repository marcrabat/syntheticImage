#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <ostream>

struct Vector2D
{
    // Constructors
    Vector2D();
	Vector2D(double a);
	Vector2D(double x_, double y_);
	Vector2D(const Vector2D &v_);

    // Member operators overload
	Vector2D operator+(const Vector2D &v) const;
	Vector2D operator-(const Vector2D &v) const;
	Vector2D operator*(const double a) const;
	Vector2D operator/(const double a) const;
	Vector2D operator-() const;

	Vector2D& operator+=(const Vector2D &v);
	Vector2D& operator-=(const Vector2D &v);
	Vector2D& operator*=(const double a);
	Vector2D& operator/=(const double a);

    // Member functions
    double length()      const;
    double lengthSq()    const;
	Vector2D normalized() const;

	void parseFromText(const char* text, const char separator);
	void setMin(const Vector2D & v);
	void setMax(const Vector2D & v);

    // Structure data
    double x, y, z;
};

// Stream insertion operator (since it takes the user-defined type at the right,
//  i.e., "Vector3D", it must be implemented as non-member
std::ostream& operator<<(std::ostream& out, const Vector2D &v);

// Dot product between two vectors
inline double dot(const Vector2D &v1, const Vector2D &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}
#endif // VECTOR3D_H
