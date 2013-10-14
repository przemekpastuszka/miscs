/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Classes derived from Primitive will describe the shape of object in 3d space
and allow to check if it intersects with given ray. You can also get a normal
of surface (perpendicular vector) at desired point. Thanks to the abstract class
adding new primitives is incredibly simple.
It also should return list of points that lie on it (for area lights use)
*/

#ifndef PRIMITIVEHELL
#define PRIMITIVEHELL

#include "vector3.h"
#include "ray.h"
#include <list>

class Primitive
{
public:
	//only Sphere, Point Plane now. But I'm going for triangles in near future
	enum PrimitiveType {SPHERE,PLANE,POINT,OTHER};
	virtual PrimitiveType GetType() const=0;

	//returns normal vector
	virtual Vector3 GetNormalAt(const Vector3& p) const=0;

	//returns true if intersection is found
	//result will contain the distant from ray origin to intersection point
	virtual bool Intersects(const Ray& r,float& result) const=0;

	//we sometimes need points to approximate the shape of the primitive
	virtual std::list<Vector3>* GetPoints(int n) const {return NULL;}

};

#endif