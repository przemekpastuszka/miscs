/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Really simple object - one primitive, one material.
Nothing interesting at all
*/

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
public:
	Simple(Primitive *pr,Material *mt) : prim(pr), m(mt)
	{
		if(pr==NULL||mt==NULL)
			Log::AddMessage("NULL pointer passed to Simple constructor",Log::ERR);
	}

	ObjectType GetType() const {return SIMPLE;}

	Vector3 GetNormalAt(const Vector3& p) const { return prim->GetNormalAt(p); }
	float GetReflectanceAt(const Vector3& p) const { return m->GetReflectance(); }

	Color CalculateColorAt(const Color& lightColor,const Vector3& lightDir,
		const Vector3& eyeDir,const Vector3& point) const
	{ 
		return m->CalculateColorAt(lightColor,lightDir,eyeDir,prim->GetNormalAt(point)); 
	}

	bool Intersects(const Ray &r, float &result) const { return prim->Intersects(r,result); }
};

#endif