/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

The simplest material - no lighting, just color. But, still, can be useful.
I think the name should be different, but, to hell, who cares? ;)
*/


#ifndef SOLIDHELL
#define SOLIDHELL

#include "material.h"

//nothing important happens inside
class Solid : public Material
{
	Color color;
public:
	Solid(const Color& c): color(c) {}

	MaterialType GetType() const {return SOLID;}

	Color CalculateColorAt(const Color& lightColor,const Vector3& lighDir,
		const Vector3& eyeDir,const Vector3& normal) const {return color;}
	float GetReflectance() const {return 0;}
};


#endif