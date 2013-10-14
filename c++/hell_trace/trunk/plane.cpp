/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Just an implementation of a plane
*/

#include "plane.h"
#include "common.h"

bool Plane::Intersects(const Ray& r,float& result) const
{
	float VD=Vector3::Dot(normal,r.Dir());
	if(VD==0)
		return false;
	float V0=-(Vector3::Dot(normal,r.Origin())+distance);
	result=V0/VD;
	if(result<EPSILON) //role of EPSILON is explained in "common.h"
		return false;
	return true;
}