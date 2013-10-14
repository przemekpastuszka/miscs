/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Essence of raytracing - lights. 
Below you can see an abstract class derived from Object. Why it is so?
Because the role of light is double:
-it lights other objects in the scene
-but also it can be percived as an object which implies it can be seen in the image 
	(and it can be even lit by other light!)
What can I add to explain this class hierarchy (or convince myself it is the right choice)?
Maybe: every light is an object and not every object is a light. It sounds quite naturally, so
following Occam's razor: "entia non sunt multiplicanda praeter necessitatem" it should be the best choice

*/

#ifndef HELLLIGHT
#define HELLLIGHT

class Light;

#include "scene.h"
#include "object.h"
#include "color.h"
#include "vector3.h"
#include <list>

class Light : public Object
{
public:
	//I decided to remove global lights, because this raytracer is designed to suite indoor scenes, so
	//they are redundant
	enum LightType {POINT,AREA,OTHER};

	//derived from object
	ObjectType GetType() const {return LIGHT;}
	virtual Vector3 GetNormalAt(const Vector3& p) const=0;
	virtual Color CalculateColorAt(const Color& lightColor,const Vector3& lightDir,
		const Vector3& eyeDir,const Vector3& point) const=0;
	virtual float GetReflectanceAt(const Vector3& p) const=0;
	virtual bool Intersects(const Ray &r, float &result) const=0;

	//Light methods
	virtual LightType GetLightType() const=0;
	

	virtual Color GetLightColorAt(const Vector3& p) const=0;
	virtual Vector3 GetLightDirAt(const Vector3& p) const=0;
	//the light can be made of other lights (like AreaLight) so you need provide list of them
	virtual const std::list<Light*>* GetLightList() const=0; 

	virtual Vector3 GetPosition() const {return Vector3();}
};

#endif
