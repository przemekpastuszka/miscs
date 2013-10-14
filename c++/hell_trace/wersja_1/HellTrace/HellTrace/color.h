#ifndef COLORHELL
#define COLORHELL

#include "vector3.h"

class Color
{
	Vector3 value;
public:
	Color() : value(Vector3(0,0,0)) {}
	Color(float r,float g,float b);
	Color(const Vector3& rgb);

	Vector3 GetRGB();

	const Color operator+(const Color& o) const;
	Color& operator+=(const Color& o);
	const Color operator-(const Color& o) const;
	Color& operator-=(const Color& o);
	const Color operator*(const Color& o) const;
	Color& operator*=(const Color& o);
	Color& operator*=(float a);
	friend const Color operator*(const Color& a,float b);
	friend const Color operator*(float b,Color& a);

	static Color Lerp(const Color& a, const Color& b,float s);
};

const Color operator*(const Color& a,float b);
const Color operator*(float b,Color& a);

#endif