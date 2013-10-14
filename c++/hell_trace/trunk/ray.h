/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Just an usual ray
*/

#ifndef HELLRAY
#define HELLRAY

#include "vector3.h"

class Ray
{
	Vector3 origin,direction;
public:
	Ray(const Vector3& o,const Vector3& dir) : origin(o),direction(dir.Normalize()) {}
	Vector3 Origin() const {return origin;}
	Vector3 Dir() const {return direction;}
	Vector3 PointAtLen(float x) const {return origin+direction*x;}
};

#endif