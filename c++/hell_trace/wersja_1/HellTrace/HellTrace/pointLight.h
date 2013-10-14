#ifndef HELLPOINTL
#define HELLPOINTL

#include "light.h"

class PointLight : public Light
{
	Color color;
	Vector3 position;
public:
	PointLight(const Color& c,const Vector3& pos) : color(c), position(pos) {}

	LightType GetType() const {return POINT;}
	Color GetColorAt(const Vector3& p) const
	{
		float sDst=(1/(p-position).SqrLength());
		sDst=sDst<1?sDst:1;
		return color*sDst;
	}
	Vector3 GetDirAt(const Vector3& p) const {return (p-position).Normalize();}
	Vector3 GetPosition() const {return position;}
};

#endif