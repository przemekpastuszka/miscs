/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Point also can be a primitive.
I had an idea to implement points as spheres with small radius, but
this is more efficient (at least I hope so)
*/

#ifndef POINTHELL
#define POINTHELL

#include "primitive.h"
#include "common.h"
#include <math.h>

class Point : public Primitive
{
	Vector3 position;
public:
	PrimitiveType GetType() const {return POINT;}

	Point(const Vector3& pos) :position(pos) {}

	//it means nothing in this context, but it must be here
	Vector3 GetNormalAt(const Vector3& p) const {return Vector3(0,1,0);}

	bool Intersects(const Ray& r,float& result) const
	{
		Vector3 temp=position-r.Origin();
		result=temp.Length();
		if(r.Dir()!=(temp/result))
			return false;
		result=sqrtf(result);
		if(result<EPSILON)
			return false;
		return true;
	}
};

#endif