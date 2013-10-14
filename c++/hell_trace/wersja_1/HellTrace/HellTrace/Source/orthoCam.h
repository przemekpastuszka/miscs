/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Will be recoded so no detailed comments yet
*/

#ifndef HELLORTHOCAM
#define HELLORTHOCAM

#include "camera.h"

class OrthographicCam : public Camera
{
	unsigned int width,height;
public:
	OrthographicCam();
	OrthographicCam(unsigned int w,unsigned int h) : width(w),height(h) {}
	Ray GetRay(unsigned int x,unsigned int y) const;
};

#endif