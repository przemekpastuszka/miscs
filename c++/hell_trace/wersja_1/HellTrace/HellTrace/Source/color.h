/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Color class is used to make an operation on colors like mixing for example.
Color is stored using 32-bit per channel.
As far clamping is used to squeeze color into RGB, but it's really naive so I may implement HDR one day.
*/

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
	bool operator==(const Color& o) const {return value==o.value;}
	bool operator!=(const Color& o) const {return value!=o.value;}
	friend const Color operator*(const Color& a,float b);
	friend const Color operator*(float b,Color& a);

	static Color Lerp(const Color& a, const Color& b,float s);
};

const Color operator*(const Color& a,float b);
const Color operator*(float b,Color& a);

#endif