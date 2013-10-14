/*
HellTrace by Przemys�aw Pastuszka
University of Wroc�aw, Poland

Will be recoded so no detailed comments yet
*/

#ifndef HELLCAMERA
#define HELLCAMERA

#include "ray.h"

class Camera
{
public:
	virtual Ray GetRay(unsigned int x,unsigned int y) const=0;
};

#endif