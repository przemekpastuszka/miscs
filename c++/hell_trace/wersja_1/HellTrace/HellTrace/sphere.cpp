#include "sphere.h"
#include "common.h"
#include <math.h>

Vector3 Sphere::GetNormalAt(const Vector3 &p) const
{
	return (p-origin).Normalize();
}

bool Sphere::Intersects(const Ray &r, float &result) const
{
	Vector3 dst=r.Origin()-origin;
	float B=Vector3::Dot(dst,r.Dir());
	float C=Vector3::Dot(dst,dst)-radius2;
	float D=B*B-C;
	if(D>0)
	{
		result=-B-sqrt(D);
		if(result<EPSILON)
			return false;
		return true;
	}
	return false;
}