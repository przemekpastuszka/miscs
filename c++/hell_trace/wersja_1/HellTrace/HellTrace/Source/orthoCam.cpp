/*
HellTrace by Przemys�aw Pastuszka
University of Wroc�aw, Poland

Will be recoded so no detailed comments yet
*/

#include "orthoCam.h"

Ray OrthographicCam::GetRay(unsigned int x, unsigned int y) const
{
	Vector3 res=Vector3(8*x/static_cast<float>(width),6*-(y/static_cast<float>(height)),0);
	res+=Vector3(-4,3,0);
	return Ray(res,Vector3(0,0,1));
}