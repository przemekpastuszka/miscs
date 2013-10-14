/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

This will be changed soon, so no detailed comments yet
*/

#include "scene.h"
#include "solid.h"
#include "simple.h"
#include "common.h"
#include <cfloat>

Scene::Scene(const Vector3 &s)
{
	size=s;
}

Scene::~Scene()
{
}

void Scene::AddObject(Object *obj)
{
	ls.push_front(obj);
	if(obj->GetType()==Object::LIGHT)
		lights.push_front(static_cast<Light*>(obj));
}


void Scene::SetSeenLights(const Vector3& point)
{
	seenLights.clear();
	for(std::list<Light*>::iterator it=lights.begin();it!=lights.end();++it)
	{
		const std::list<Light*>* insides=(*it)->GetLightList();
		for(std::list<Light*>::const_iterator j=insides->begin();j!=insides->end();++j)
		{
			Vector3 p=(*j)->GetPosition();
			Ray a(p,point-p);
			float pDst=(point-p).Length();
			bool isOK=true;
			for(std::list<Object*>::const_iterator z=ls.begin();z!=ls.end();++z)
			{
				float result;
				if((*z)->Intersects(a,result))
					if(result<pDst-EPSILON)
					{
						isOK=false;
						break;
					}
			}
			if(isOK)
				seenLights.push_back(*j);
		}
	}
	it=seenLights.begin();
}


const Light* Scene::NextLight()
{
	if(it==seenLights.end())
		return NULL;
	Light* res=*it;
	it++;
	return res;
}


bool Scene::ClosestInt(const Ray &a, Scene::Intersection &result) const
{
	result.dst=FLT_MAX;
	result.o=NULL;
	bool found=false;
	for(std::list<Object*>::const_iterator it=ls.begin();it!=ls.end();++it)
	{
		float dst;
		if((*it)->Intersects(a,dst))
			if(dst<result.dst)
			{
				result.dst=dst;
				result.o=*it;
				found=true;
			}
	}
	return found;
}