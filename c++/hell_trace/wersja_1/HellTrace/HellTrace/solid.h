#ifndef SOLIDHELL
#define SOLIDHELL

#include "material.h"

class Solid : public Material
{
	Color color;
	float diffuse,specular;
	int shineness;
public:
	Solid(const Color& c,float diff,float spec,int shine) : color(c),diffuse(diff), specular(spec),shineness(shine) {}
	MaterialType GetType() const {return SOLID;}
	Vector3 GetNormalAt(const Vector3& p) const {return Vector3(0,1,0);}
	Color GetColorAt(const Vector3& p) const {return color;}
	float GetDiffuseAt(const Vector3& p) const {return diffuse;}
	float GetSpecularAt(const Vector3& p) const {return specular;}
	int GetShinenessAt(const Vector3& p) const {return shineness;}
};

#endif