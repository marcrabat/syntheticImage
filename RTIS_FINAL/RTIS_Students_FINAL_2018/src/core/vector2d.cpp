#include "vector2d.h"

Vector2D::Vector2D() : x(0), y(0)
{
}

Vector2D::Vector2D(double a) : x(a), y(a)
{
}

Vector2D::Vector2D(double x_, double y_) : x(x_), y(y_)
{
}

Vector2D::Vector2D(const Vector2D &v_) : x(v_.x), y(v_.y)
{
}

// Sum two vectors and return the result as a new object
Vector2D Vector2D::operator+(const Vector2D &v) const
{
    return Vector2D(x + v.x, y + v.y);
}

// Subtract two vectors and return the result as a new object
Vector2D Vector2D::operator-(const Vector2D &v) const
{
    return Vector2D(x - v.x, y - v.y);
}

// Return the negative of the vector as a new object
Vector2D Vector2D::operator-() const
{
    return Vector2D(-x, -y);
}

// Multiply a vector by a scalar and return the result as a new object
Vector2D Vector2D::operator*(const double a) const
{
    return Vector2D(x*a, y*a);
}

// Divide a vector by a scalar and return the result as a new object
Vector2D Vector2D::operator/(const double a) const
{
    return Vector2D(x/a, y/a);
}

// Add a vector to the current one
Vector2D& Vector2D::operator+=(const Vector2D &v)
{
    x += v.x;
    y += v.y;

    return *this;
}

// Subtract a vector to the current one
Vector2D& Vector2D::operator-=(const Vector2D &v)
{
    x -= v.x;
    y -= v.y;

    return *this;
}

// Multiply the current vector by a scalar
Vector2D& Vector2D::operator*=(const double a)
{
    x *= a;
    y *= a;

    return *this;
}

// Divide the current vector by a scalar
Vector2D& Vector2D::operator/=(const double a)
{
    x /= a;
    y /= a;

    return *this;
}

// Squared length of the current vector
double Vector2D::lengthSq() const
{
    return x*x + y*y;
}

// Length of the current vector
double Vector2D::length() const
{
    //[T] : check line 572 of vector.h in mitsuba
    return std::sqrt(lengthSq());
}

// Returns the normalized version of the current vector
Vector2D Vector2D::normalized() const
{
    return (*this)/length();
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& out, const Vector2D &v)
{
    out << "[" << v.x << ", " << v.y << "]";
    return out;
}

void Vector2D::setMin(const Vector2D & v)
{
	if (v.x < x) x = v.x;
	if (v.y < y) y = v.y;
}

void Vector2D::setMax(const Vector2D & v)
{
	if (v.x > x) x = v.x;
	if (v.y > y) y = v.y;
}

void Vector2D::parseFromText(const char* text, const char separator)
{
	int pos = 0;
	char num[255];
	const char* start = text;
	const char* current = text;

	while (1)
	{
		if (*current == separator || (*current == '\0' && current != text))
		{
			strncpy(num, start, current - start);
			num[current - start] = '\0';
			start = current + 1;
			if (num[0] != 'x') //¿?
				switch (pos)
				{
				case 0: x = (float)atof(num); break;
				case 1: y = (float)atof(num); break;
				default: return; break;
				}

			++pos;
			if (*current == '\0')
				break;
		}

		++current;
	}
}