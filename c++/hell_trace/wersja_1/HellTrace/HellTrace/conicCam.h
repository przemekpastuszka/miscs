#ifndef HELLCONICCAM
#define HELLCONICCAM

#include "camera.h"

class ConicCam : public Camera
{
	unsigned int width,height;
public:
	ConicCam();
	ConicCam(unsigned int w,unsigned int h) : width(w),height(h) {}
	Ray GetRay(unsigned int x,unsigned int y) const;
};

#endif