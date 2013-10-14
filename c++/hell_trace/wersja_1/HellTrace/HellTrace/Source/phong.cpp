/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Just implementing the Blinn-Phong
*/

#include "phong.h"
#include <math.h>
#include "utilities.h"

Color Phong::CalculateColorAt(const Color& lightColor,const Vector3& lightDir,
		const Vector3& eyeDir,const Vector3& normal) const
{
	//diffuse part (calculate the cosine between normal and light direction)
	float dot=Vector3::Dot(-normal,lightDir);
	dot=dot>0?dot:0; //if dot is <0 the light is on the wrong side of the surface. So no lighting is performed
	Color result=color*lightColor*dot*diffuse;

	//Blinn-Phong specular 
	//(I'm too lasy to comment each line of code, so if you want to know more - google it)
	Vector3 blinn=lightDir+eyeDir;
	float temp=Vector3::Dot(blinn,blinn);
	if(temp!=0)
	{
		temp=sqrt(temp);
		blinn=(1/temp)*blinn;
		float blinnTerm=Vector3::Dot(blinn,-normal);
		blinnTerm=blinnTerm>0?blinnTerm:0;
		result+=specColor*Power(blinnTerm,specPower)*blinnTerm*lightColor;
	}
	return result;
}