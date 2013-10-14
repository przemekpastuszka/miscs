#ifndef SCENEHELL
#define SCENEHELL

#include "object.h"
#include "light.h"
#include "vector3.h"
#include <list>

class Scene
{
private:
	std::list<Object*> ls;
	std::list<Light*> lights;
	std::list<Light*>::iterator it;
	Vector3 size;
	float ambient;
public:
	struct Intersection
	{
		Object* o;
		float dst;
	};

	Scene(const Vector3& s,float am);
	~Scene();

	float GetAmbient() const;
	void AddObject(Object* obj);
	void AddLight(Light* l);
	bool ClosestInt(const Ray& a,Intersection& result) const;
	//bool Intersects(const Ray& a);
	bool IsLit(const Vector3& p,const Light* l) const;
	void SetFirstLight();
	const Light* GetLight();
	
};

#endif