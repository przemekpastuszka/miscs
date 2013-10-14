#include "vector3.h"
#include <math.h>

const Vector3 Vector3::operator +(const Vector3 &a) const
{
	return this->Translate(a);
}

const Vector3 Vector3::operator -() const
{
	return Vector3(-x,-y,-z);
}

const Vector3 Vector3::operator -(const Vector3 &a) const
{
	return (*this)+(-a);
}

const Vector3 Vector3::Translate(float x,float y,float z) const
{
	return Vector3(this->x+x,this->y+y,this->z+z);
}

const Vector3 Vector3::Translate(const Vector3& a) const
{
	return Vector3(x+a.x,y+a.y,z+a.z);
}

const Vector3 Vector3::Scale(float s) const
{
	return Vector3(x*s,y*s,z*s);
}

const Vector3 Vector3::Scale(float x, float y, float z) const
{
	return Vector3(this->x*x,this->y*y,this->z*z);
}

const Vector3 Vector3::Scale(const Vector3& a) const
{
	return Vector3(x*a.x,y*a.y,z*a.z);
}

const Vector3 Vector3::RotateY(float angle) const
{
	return Vector3(x*(float)cos(angle)-z*(float)sin(angle),y,x*(float)sin(angle)+z*(float)cos(angle));
}

float Vector3::Length() const
{
	return (float)sqrt(x*x+y*y+z*z);
}

const Vector3 Vector3::Normalize() const
{
	float length=Length();
	return Vector3(x/length,y/length,z/length);
}

float Vector3::Dot(const Vector3& a,const Vector3& b)
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

const Vector3 Vector3::Cross(const Vector3& a,const Vector3& b)
{
	return Vector3(a.y*b.z-a.z*b.y,-a.x*b.z+a.z*b.x,a.x*b.y-a.y*b.x);
}


const Vector3 operator*(const Vector3& a,float b)
{
	return Vector3(a.x*b,a.y*b,a.z*b);
}

const Vector3 operator/(const Vector3& a,float b)
{
	return Vector3(a.x/b,a.y/b,a.z/b);
}
const Vector3 operator*(float b,const Vector3& a)
{
	return Vector3(a.x*b,a.y*b,a.z*b);
}

const Vector3 operator/(float b,const Vector3& a)
{
	return Vector3(b/a.x,b/a.y,b/a.z);
}

Vector3& Vector3::operator *=(float b)
{
	x*=b;y*=b;z*=b;
	return (*this);
}

Vector3& Vector3::operator /=(float b)
{
	x/=b;y/=b;z/=b;
	return (*this);
}

Vector3& Vector3::operator +=(const Vector3& a)
{
	x+=a.x;y+=a.y;z+=a.z;
	return (*this);
}

Vector3& Vector3::operator -=(const Vector3& a)
{
	x-=a.x;y-=a.y;z-=a.z;
	return (*this);
}

float Vector3::SqrLength() const
{
	return x*x+y*y+z*z;
}

const Vector3 Vector3::GetMin(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x<b.x?a.x:b.x,a.y<b.y?a.y:b.y,a.z<b.z?a.z:b.z);
}

const Vector3 Vector3::GetMax(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x>b.x?a.x:b.x,a.y>b.y?a.y:b.y,a.z>b.z?a.z:b.z);
}

const Vector3 Vector3::Clamp(const Vector3& a,float min,float max)
{
	return GetMax(Vector3(min),GetMin(Vector3(max),a));
}