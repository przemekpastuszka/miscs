#ifndef HELLLIGHT
#define HELLLIGHT

#include "color.h"
#include "vector3.h"

class Light
{
public:
	enum LightType {GLOBAL,POINT};

	virtual LightType GetType() const=0;
	virtual Color GetColorAt(const Vector3& p) const=0;
	virtual Vector3 GetDirAt(const Vector3& p) const=0; 
};

#endif