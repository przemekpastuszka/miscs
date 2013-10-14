#ifndef OBJECTHELL
#define OBJECTHELL

#include "vector3.h"
#include "color.h"
#include "ray.h"

class Object
{
public:
	enum ObjectType {SIMPLE};
	virtual ObjectType GetType() const=0;
	virtual bool IsLight() const=0;
	virtual Vector3 GetNormalAt(const Vector3& p) const=0;
	virtual Color GetColorAt(const Vector3& p) const=0;
	virtual float GetDiffuseAt(const Vector3& p) const=0;
	virtual float GetSpecularAt(const Vector3& p) const=0;
	virtual int GetShinenessAt(const Vector3& p) const=0;
	virtual bool Intersects(const Ray &r, float &result) const=0;
};

#endif