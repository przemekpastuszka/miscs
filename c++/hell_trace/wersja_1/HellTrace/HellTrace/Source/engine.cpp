/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Here comes the raytracing algorithm:
-For each pixel of the image produce ray and pass it to tracing function
-while multiplier coeff is greater than 0 and the ray hasn't been traced enough times yet
	-find the closest intersection with the ray
	-for each light visible from intersection point calculate the color of this point
	-calculate reflected ray, set it to be traced and update the multiplier
*/

#include "engine.h"
#include "common.h"
#include "utilities.h"
#include <math.h>
#include <cfloat>

Engine::~Engine()
{
}

PixelBuff::Pixel Engine::RayTrace(const Ray &a,int dpt) const
{
	//initialization
	PixelBuff::Pixel result;
	result.depth=FLT_MAX;
	float multiplier=1;
	Ray current=a;
	for(int i=0;i<dpt&&multiplier!=0;++i)
	{
		Scene::Intersection intersection;
		if(!scene->ClosestInt(current,intersection))
			return result; //there was no intersection, so end tracing

		if(result.depth==FLT_MAX) //depth hasn't been set yet so do it
			result.depth=intersection.dst;

		Vector3 point=current.PointAtLen(intersection.dst); //here is the intersection point

		scene->SetSeenLights(point); //check which lights can be seen from point
		const Light* t;
		while((t=scene->NextLight())!=NULL) //for each light do:
		{
			Vector3 lightDir=t->GetLightDirAt(point);
			Color lightColor=t->GetLightColorAt(point);
			//add calculated color, multiplied by multiplier, to result.color
			result.color+=intersection.o->CalculateColorAt(lightColor,lightDir,current.Dir(),point)*multiplier;
		}
		//calculate reflected ray
		Vector3 normal=-intersection.o->GetNormalAt(point);
		current=Ray(point,a.Dir()-2*normal*Vector3::Dot(current.Dir(),normal));
		multiplier*=intersection.o->GetReflectanceAt(point); //multiply by reflectance
	}
	return result;
}

void Engine::Render(const Camera* c, PixelBuff &p) const
{
	//just doing tracing for each pixel
	for(unsigned int i=0;i<p.Width();++i)
		for(unsigned int j=0;j<p.Height();++j)
		{
			Ray r=c->GetRay(i,j);
			p.Set(i,j,RayTrace(r,TRACINGDPT));
		}
}