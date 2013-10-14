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