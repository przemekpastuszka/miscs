/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Classes derived from Material will contain informations about surfaces used to cover the scene objects
(like reflectance,diffuse coefficients, color and so on) and will also compute point color based on
light color, light ray direction, eye direction (traced ray) and also a normal of the surface, which the
material is covering. This computation has been moved from Engine class, because thanks to this it is far
easier to implement new lighting models
*/

#ifndef HELLMATERIAL
#define HELLMATERIAL

#include "vector3.h"
#include "color.h"
#include "resource.h"

class Material : public Resource
{
public:
	ResourceType GetResourceType() const {return MATERIAL;}

	//only Solid and Blinn-Phong models so far
	enum MaterialType {SOLID,PHONG,OTHER};
	virtual MaterialType GetType() const=0; //you can always check what is the type of the material
	
	//here is the computation part
	virtual Color CalculateColorAt(const Color& lightColor,const Vector3& lighDir,
		const Vector3& eyeDir,const Vector3& normal) const=0;
	virtual float GetReflectance() const=0; //returns the reflectance of a surface
};

#endif
