#include "scene.h"
#include "solid.h"
#include "simple.h"
#include "sphere.h"
#include "pointLight.h"
#include <cfloat>

Scene::Scene(const Vector3 &s,float am)
{
	size=s;
	ambient=am;
}

Scene::~Scene()
{
}

void Scene::AddObject(Object *obj)
{
	ls.push_front(obj);
}

void Scene::AddLight(Light* l)
{
	if(l->GetType()==Light::POINT)
	{
		PointLight* p=static_cast<PointLight*>(l);
		ls.push_front(new Simple(new Sphere(p->GetPosition(),0.1),new Solid(p->GetColorAt(p->GetPosition()),1,0,0),true));
	}
	lights.push_front(l);
}

void Scene::SetFirstLight()
{
	it=lights.begin();
}

float Scene::GetAmbient() const
{
	return ambient;
}

const Light* Scene::GetLight()
{
	if(it==lights.end())
		return NULL;
	Light* res=*it;
	it++;
	return res;
}

/*bool Scene::Intersects(const Ray& a)
{
	for(std::list<Object*>::const_iterator it=ls.begin();it!=ls.end();++it)
	{
		if((*it)->IsLight())
			continue;
		float dst;
		if((*it)->Intersects(a,dst))
			return true;
	}
	return false;
}*/

bool Scene::IsLit(const Vector3 &p, const Light *l) const
{
	Ray a=Ray(p,-l->GetDirAt(p));
	float pDst;
	if(l->GetType()==Light::POINT)
	{
		const PointLight* pl=static_cast<const PointLight*>(l);
		pDst=(pl->GetPosition()-p).Length();
	}
	for(std::list<Object*>::const_iterator it=ls.begin();it!=ls.end();++it)
	{
		if((*it)->IsLight())
			continue;
		float dst;
		if((*it)->Intersects(a,dst))
		{
			if(l->GetType()==Light::POINT&&dst>pDst)
				continue;
			return false;
		}
	}
	return true;
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