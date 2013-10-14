#ifndef HELLMATERIAL
#define HELLMATERIAL

#include "vector3.h"
#include "color.h"

class Material
{
public:
	enum MaterialType {SOLID};
	virtual MaterialType GetType() const=0;
	virtual Vector3 GetNormalAt(const Vector3& p) const=0;
	virtual Color GetColorAt(const Vector3& p) const=0;
	virtual float GetDiffuseAt(const Vector3& p) const=0;
	virtual float GetSpecularAt(const Vector3& p) const=0;
	virtual int GetShinenessAt(const Vector3& p) const=0;
};

#endif