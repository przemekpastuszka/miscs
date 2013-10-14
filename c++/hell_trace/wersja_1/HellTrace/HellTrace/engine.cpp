#include "engine.h"
#include "common.h"
#include "utilities.h"
#include <math.h>

Engine::~Engine()
{
}

Color Engine::RayTrace(const Ray &a,int dpt) const
{
	if(dpt>TRACINGDPT)
		return Color();
	Scene::Intersection intersection;
	if(scene->ClosestInt(a,intersection))
	{
		
		Color result;
		Vector3 point=a.PointAtLen(intersection.dst);
		Vector3 normal=intersection.o->GetNormalAt(point);
		normal=-normal;

		if(intersection.o->IsLight())
			return intersection.o->GetColorAt(point);

		//reflected vector
		Ray casted(point,a.Dir()-2*normal*Vector3::Dot(a.Dir(),normal));
		float diff=intersection.o->GetDiffuseAt(point);

		scene->SetFirstLight();
		const Light* t;
		while((t=scene->GetLight())!=NULL)
		{
			
			if(!scene->IsLit(point,t))
				continue;//is in shadow
			
			Color lightColor=t->GetColorAt(point);

			//material color
			float dot = Vector3::Dot(normal,t->GetDirAt(point));
			dot=dot>0?dot:0;
			result+=intersection.o->GetColorAt(point)*dot*diff*lightColor;

			//specular
			Vector3 blinn=t->GetDirAt(point)+a.Dir();
			float temp=sqrt(Vector3::Dot(blinn,blinn));
			if(temp!=0)
			{
				blinn=(1/temp)*blinn;
				float blinnTerm=Vector3::Dot(blinn,normal);
				blinnTerm=blinnTerm>0?blinnTerm:0;
				blinnTerm=intersection.o->GetSpecularAt(point)*Power(blinnTerm,intersection.o->GetShinenessAt(point));
				result+=lightColor*blinnTerm;
			}
		}

		//reflection
		Color reflectedC;
		if(diff<1)
			reflectedC=RayTrace(casted,dpt+1);
		result+=reflectedC*(1-diff);
		result+=intersection.o->GetColorAt(point)*scene->GetAmbient()*intersection.o->GetDiffuseAt(point);
		return result;		
	}
	return Color();
}

void Engine::Render(const Camera* c, PixelBuff &p) const
{
	for(int i=0;i<p.Width();++i)
		for(int j=0;j<p.Height();++j)
		{
			Ray r=c->GetRay(i,j);
			p.Set(i,j,RayTrace(r,1));
		}
}