/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

All you can see in the scene is an object.
Classes derived from Object should allow determining normal vector of the surface at given point,
calculate color, reflectance or checking intersections. But all those methods are implemented by
Material and Primitive. So what's the point in creating a new class? Paradoxally, it helps increase
flexibility of the code, because implementing complex geometric objects (made of different primitives even),
blending materials, bump mapping and so on it's now easy.
*/

#ifndef OBJECTHELL
#define OBJECTHELL

#include "vector3.h"
#include "color.h"
#include "ray.h"
#include "resource.h"

class Object : public Resource
{
public:
	ResourceType GetResourceType() const {return OBJECT;}

	//Light is also an object! In the very first version of HellTracer it was not, but I wasn't fond
	//of that solution, so I started to think hard and I'm now quite sure, that current code is better :)
	enum ObjectType {SIMPLE,LIGHT,OTHER};
	virtual ObjectType GetType() const=0;

	//described at the top
	virtual Vector3 GetNormalAt(const Vector3& p) const=0;
	virtual Color CalculateColorAt(const Color& lightColor,const Vector3& lightDir,
		const Vector3& eyeDir,const Vector3& point) const=0;
	virtual float GetReflectanceAt(const Vector3& p) const=0;
	virtual bool Intersects(const Ray &r, float &result) const=0;
};

#endif
