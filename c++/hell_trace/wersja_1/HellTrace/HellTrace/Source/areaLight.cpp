/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Implementation of area light
*/

#include "areaLight.h"
#include "pointLight.h"

AreaLight::AreaLight(const Color &c, const Primitive *p, int apprPoints)  : color(c),prim(p)
{
	//just making point lights using points generated on the primitive
	std::list<Vector3>* pts=prim->GetPoints(apprPoints);
	pointLs=new std::list<Light*>();
	for(std::list<Vector3>::iterator it=pts->begin();it!=pts->end();++it)
		pointLs->push_back(new PointLight(color*(1/static_cast<float>(apprPoints)),*it,NULL));
	delete pts;
}

AreaLight::~AreaLight()
{
	for(std::list<Light*>::iterator it=pointLs->begin();it!=pointLs->end();++it)
		delete (*it);
	delete pointLs;
}

Color AreaLight::CalculateColorAt(const Color& lightColor,const Vector3& lightDir,
		const Vector3& eyeDir,const Vector3& point) const
{
	return color; //behaves like Solid material
}

