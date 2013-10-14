#ifndef PRIMITIVEHELL
#define PRIMITIVEHELL

#include "vector3.h"
#include "ray.h"

class Primitive
{
public:
	enum PrimitiveType {SPHERE,PLANE};
	virtual PrimitiveType GetType() const=0;
	virtual Vector3 GetNormalAt(const Vector3& p) const=0;
	virtual bool Intersects(const Ray& r,float& result) const=0;
};

#endif