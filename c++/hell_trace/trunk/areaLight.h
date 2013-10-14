/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Area light which is approximated by set of point lights
Why is it better than point lights? 
Just look at the soft shadows and you'll know why ;)
*/

#ifndef HELLAREA
#define HELLAREA

#include "light.h"
#include "primitive.h"
#include <list>

class AreaLight : public Light
{
	Color color;
	const Primitive *prim;
	std::list<Light*> *pointLs; //list of point lights
public:
	AreaLight(const Color& c, const Primitive* p, int apprPoints);
	~AreaLight();

	LightType GetLightType() const {return AREA;}

	//behaves like Solid material
	Vector3 GetNormalAt(const Vector3& p) const {return Vector3(0,1,0);}
	Color CalculateColorAt(const Color& lightColor,const Vector3& lightDir,
		const Vector3& eyeDir,const Vector3& point) const;
	float GetReflectanceAt(const Vector3& p) const {return 0;}
	bool Intersects(const Ray &r, float &result) const {return prim->Intersects(r,result);}

	Color GetLightColorAt(const Vector3& p) const {return color;}
	Vector3 GetLightDirAt(const Vector3& p) const {return Vector3();}
	const std::list<Light*>* GetLightList() const {return pointLs;}
};

#endif