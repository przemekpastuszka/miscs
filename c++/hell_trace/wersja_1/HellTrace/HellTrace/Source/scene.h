/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

This will be changed soon, so no detailed comments yet
*/

#ifndef SCENEHELL
#define SCENEHELL

class Scene;

#include "object.h"
#include "light.h"
#include "vector3.h"
#include <list>

class Scene
{
private:
	std::list<Object*> ls;
	std::list<Light*> lights;
	std::list<Light*> seenLights;
	std::list<Light*>::iterator it;
	Vector3 size;
public:
	struct Intersection
	{
		Object* o;
		float dst;
	};

	Scene(const Vector3& s);
	~Scene();

	void AddObject(Object* obj);
	bool ClosestInt(const Ray& a,Intersection& result) const;
	
	void SetSeenLights(const Vector3& p);
	const Light* NextLight();
};

#endif