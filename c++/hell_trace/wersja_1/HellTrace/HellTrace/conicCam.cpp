#include "conicCam.h"

Ray ConicCam::GetRay(unsigned int x,unsigned int y) const
{
	//Vector3 res=Vector3(x/100.0f,-(y/100.0f),0);
	//res+=Vector3(-(width/200.0f),height/200,0);
	Vector3 res=Vector3(8*x/static_cast<float>(width),6*-(y/static_cast<float>(height)),0);
	res+=Vector3(-4,3,0);
	return Ray(res,res-Vector3(0,0,-5));
}