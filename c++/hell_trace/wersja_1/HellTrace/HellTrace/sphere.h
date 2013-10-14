#ifndef SPHEREHELL
#define SPHEREHELL

#include "primitive.h"

class Sphere : public Primitive
{
	Vector3 origin;
	float radius,radius2;
public:
	Sphere(const Vector3& o,float r) : origin(o),radius(r),radius2(r*r) {}
	PrimitiveType GetType() const {return SPHERE;}
	Vector3 GetNormalAt(const Vector3& p) const;
	bool Intersects(const Ray& r,float& result) const;
};

#endif