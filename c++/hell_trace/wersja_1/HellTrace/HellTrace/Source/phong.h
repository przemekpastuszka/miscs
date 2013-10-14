/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Material that provides Blinn-Phong model. I can imagine better models, but I think
it's good (read: simple enough) thing to start with. And it simulates plastic surfaces
quite realistically. Which is nice.
*/

#ifndef PHONGHELL
#define PHONGHELL

#include "material.h"

class Phong : public Material
{
	//here we have surface and specular colors,
	//diffuse and reflectance coefficients and power
	//which will be used to calculate specular
	Color color,specColor;
	float diffuse,reflectance;
	int specPower;
public: //nothing happens below
	Phong(const Color& c,const Color& specC,float diff,float ref,int specPow) 
		: color(c), specColor(specC),diffuse(diff), reflectance(ref),specPower(specPow) {}

	MaterialType GetType() const {return PHONG;}

	Color CalculateColorAt(const Color& lightColor,const Vector3& lightDir,
		const Vector3& eyeDir,const Vector3& normal) const;
	float GetReflectance() const {return reflectance;}
};

#endif