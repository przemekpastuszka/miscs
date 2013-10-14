/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Infinite plane
*/

#ifndef HELLPLANE
#define HELLPLANE

#include "primitive.h"

class Plane : public Primitive
{
	//plane is described as:
	Vector3 normal; //normalized perpendicular vector (pointing at (0,0,0))
	float distance; //distance from (0,0,0) to the plane
public:
	Plane(const Vector3& n,float d) : normal(n.Normalize()), distance(d/n.Length()) {}
	PrimitiveType GetType() const {return PLANE;}
	Vector3 GetNormalAt(const Vector3& p) const {return normal;}
	bool Intersects(const Ray& r,float& result) const;
};

#endif