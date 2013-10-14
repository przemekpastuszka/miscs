#ifndef SIMPLEHELL
#define SIMPLEHELL

#include "object.h"
#include "primitive.h"
#include "material.h"
#include "log.h"

class Simple : public Object
{
	Primitive* prim;
	Material* m;
	bool isLight;
public:
	Simple(Primitive *pr,Material *mt,bool light)
	{
		if(pr==NULL||mt==NULL)
			Log::AddMessage("NULL pointer passed to Simple constructor",Log::ERR);
		prim=pr;
		m=mt;
		isLight=light;
	}
	~Simple()
	{
		delete prim;
	}
	bool IsLight() const
	{
		return isLight;
	}
	ObjectType GetType() const {return SIMPLE;}
	Vector3 GetNormalAt(const Vector3& p) const
	{
		return prim->GetNormalAt(p);
	}
	Color GetColorAt(const Vector3& p) const
	{
		return m->GetColorAt(Vector3(0,0,0));
	}
	float GetDiffuseAt(const Vector3& p) const
	{
		return m->GetDiffuseAt(Vector3(0,0,0));
	}
	float GetSpecularAt(const Vector3& p) const
	{
		return m->GetSpecularAt(Vector3(0,0,0));
	}
	int GetShinenessAt(const Vector3& p) const
	{
		return m->GetShinenessAt(Vector3(0,0,0));
	}
	bool Intersects(const Ray &r, float &result) const
	{
		return prim->Intersects(r,result);
	}
};

#endif