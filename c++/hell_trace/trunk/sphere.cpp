/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Implementation of Sphere
*/
#define _USE_MATH_DEFINES
#include "sphere.h"
#include "common.h"
#include <math.h>


Vector3 Sphere::GetNormalAt(const Vector3 &p) const
{
	//when considering spheres normal vector is normalized vector from sphere origin to point
	//on its surface
	return (p-origin).Normalize();
}

bool Sphere::Intersects(const Ray &r, float &result) const
{
	//solve the quadratic equation
	Vector3 dst=r.Origin()-origin;
	float B=Vector3::Dot(dst,r.Dir());
	float C=Vector3::Dot(dst,dst)-radius2;
	float D=B*B-C;
	if(D>0)
	{
		result=-B-sqrt(D);
		if(result<EPSILON) //role of EPSILON is explained in "common.h"
			return false;
		return true;
	}
	return false;
}

std::list<Vector3>* Sphere::GetPoints(int n) const
{
	std::list<Vector3>* result=new std::list<Vector3>();

	float inc=(float)M_PI*(3-sqrtf(5));
	float off=static_cast<float>(2)/n;
	for(int k=0;k<=n;++k)
	{
		float y=k*off-1+(off/2);
		float r=sqrt(1-y*y);
		float phi=k*inc;
		Vector3 temp((float)cos(phi)*r,y,(float)sin(phi)*r);
		result->push_back(temp*radius+origin);
	}

	return result;
}