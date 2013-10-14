/*#ifndef HELLGLOBALLIGHT
#define HELLGLOBALLIGHT

#include "light.h"

class GlobalLight : public Light
{
	Color color;
	Vector3 direction;
public:
	GlobalLight(const Color& c,const Vector3& dir) : color(c),direction(dir.Normalize()) {}

	LightType GetType() const {return GLOBAL;}
	Color GetColorAt(const Vector3& p) const {return color;}
	Vector3 GetDirAt(const Vector3& p) const {return direction;}
};

#endif*/