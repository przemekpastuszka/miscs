#ifndef HELLPLANE
#define HELLPLANE

#include "primitive.h"

class Plane : public Primitive
{
	Vector3 normal;
	float distance;
public:
	Plane(const Vector3& n,float d) : normal(n.Normalize()), distance(d/n.Length()) {}
	PrimitiveType GetType() const {return PLANE;}
	Vector3 GetNormalAt(const Vector3& p) const {return normal;}
	bool Intersects(const Ray& r,float& result) const;
};

#endif