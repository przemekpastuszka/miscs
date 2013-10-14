/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Nothing interesting here
*/

#include "color.h"
#include <math.h>

Color::Color(float r,float g,float b)
{
	value=Vector3(r,g,b);
}

Color::Color(const Vector3& rgb)
{
	value=rgb;
}

Vector3 Color::GetRGB()
{
	return Vector3::Clamp(Vector3((int)(255*value.x),(int)(255*value.y),(int)(255*value.z)),0,255);
	/*float exposure=-4;
	Vector3 e(1.0-expf(exposure*value.r),1.0-expf(exposure*value.g),1.0-expf(exposure*value.b));
	return Vector3::Clamp(Vector3((int)(255*e.x),(int)(255*e.y),(int)(255*e.z)),0,255);*/
}

const Color Color::operator +(const Color &o) const
{
	return Color(value+o.value);
}

const Color Color::operator -(const Color &o) const
{
	return Color(value-o.value);
}

const Color Color::operator *(const Color &o) const
{
	return Color(value.Scale(o.value));
}

Color& Color::operator +=(const Color &o)
{
	value+=o.value;
	return *this;
}

Color& Color::operator -=(const Color &o)
{
	value-=o.value;
	return *this;
}


Color& Color::operator *=(float s)
{
	value*=s;
	return *this;
}

Color& Color::operator *=(const Color& o)
{
	value=value.Scale(o.value);
	return *this;
}

const Color operator*(const Color& a,float b)
{
	return Color(a.value*b);
}

const Color operator*(float b,Color& a)
{
	return Color(a.value*b);
}

Color Color::Lerp(const Color &a, const Color &b, float s)
{
	return a*s+b*(1-s);
}