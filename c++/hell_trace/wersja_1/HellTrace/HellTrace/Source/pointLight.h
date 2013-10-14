/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Point light - not very complicated.
Things worth to be noticed: 
-I added an attenuation - now the color at point is multiplied by
	the inversion of square length between this point and light position
*/

#ifndef HELLPOINTL
#define HELLPOINTL

#include "light.h"
#include "primitive.h"

class PointLight : public Light
{
	Color color;
	const Primitive *prim; //although it is point light it can have any shape
	Vector3 position; //and still you need to specify the origin
	std::list<Light*> ls;
public:
	PointLight(const Color& c,const Vector3& pos, const Primitive* p) : color(c), position(pos),prim(p) 
	{
		//point light consists of point light! And that's all!
		ls.push_back(this);
	}

	LightType GetLightType() const {return POINT;}

	//behaves like Solid material
	Vector3 GetNormalAt(const Vector3& p) const {return Vector3(0,1,0);}
	Color CalculateColorAt(const Color& lightColor,const Vector3& lightDir,
		const Vector3& eyeDir,const Vector3& point) const {return color;}
	float GetReflectanceAt(const Vector3& p) const {return 0;}
	bool Intersects(const Ray &r, float &result) const 
	{
		if(prim!=NULL)
			return prim->Intersects(r,result);
		return false;
	}

	Color GetLightColorAt(const Vector3& p) const
	{
		float sDst=(1/(p-position).SqrLength()); //inverted square length
		sDst=sDst<1?sDst:1; //multiplier should not be grater than 0

		return color*sDst;
	}
	Vector3 GetLightDirAt(const Vector3& p) const {return (p-position).Normalize();}
	const std::list<Light*>* GetLightList() const {return &ls;} 

	Vector3 GetPosition() const {return position;}
};

#endif